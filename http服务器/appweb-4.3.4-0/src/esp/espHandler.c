/*
    espHandler.c -- Embedded Server Pages (ESP) handler

    Copyright (c) All Rights Reserved. See copyright notice at the bottom of the file.
 */

/********************************** Includes **********************************/

#include    "appweb.h"

#if BIT_PACK_ESP
#include    "esp.h"
#include    "edi.h"

/************************************* Local **********************************/
/*
    Singleton ESP control structure
 */
static Esp *esp;

/************************************ Forward *********************************/

static EspRoute *allocEspRoute(HttpRoute *loc);
static int espDbDirective(MaState *state, cchar *key, cchar *value);
static int espEnvDirective(MaState *state, cchar *key, cchar *value);
static int espLoadDirective(MaState *state, cchar *key, cchar *value);
static EspRoute *getEroute(HttpRoute *route);
static int loadApp(HttpConn *conn, int *updated);
static void manageEsp(Esp *esp, int flags);
static void manageReq(EspReq *req, int flags);
static int runAction(HttpConn *conn);
static int setupEspApp(HttpRoute *route, cchar *name, cchar *path);
static void setRouteDirs(MaState *state, cchar *kind);
static int unloadEsp(MprModule *mp);
static bool viewExists(HttpConn *conn);

#if !BIT_STATIC
static char *getControllerEntry(cchar *controllerName);
#endif

/************************************* Code ***********************************/
/*
    Open an instance of the ESP for a new request
 */
static void openEsp(HttpQueue *q)
{
    HttpConn    *conn;
    HttpRx      *rx;
    HttpRoute   *route;
    EspRoute    *eroute;
    EspReq      *req;

    conn = q->conn;
    rx = conn->rx;

    /*
        If unloading a module, this lock will cause a wait here while ESP applications are reloaded.
     */
    lock(esp);
    esp->inUse++;
    unlock(esp);

    if (rx->flags & (HTTP_OPTIONS | HTTP_TRACE)) {
        /*
            ESP accepts all methods if there is a registered route. However, we only advertise the standard methods.
         */
        httpHandleOptionsTrace(q->conn, "DELETE,GET,HEAD,POST,PUT");
    } else {
        if ((req = mprAllocObj(EspReq, manageReq)) == 0) {
            httpMemoryError(conn);
            return;
        }
        /*
            Find the ESP route configuration. Search up the route parent chain
         */
        for (eroute = 0, route = rx->route; route; route = route->parent) {
            if (route->eroute) {
                eroute = route->eroute;
                break;
            }
        }
        if (!route) {
            httpError(conn, 0, "Cannot find a suitable ESP route configuration in appweb.conf");
            return;
        }
        if (!eroute) {
            //  MOB - should be saved for future similar requests (locking too)
            eroute = allocEspRoute(route);
            return;
        }
        conn->data = req;
        req->esp = esp;
        req->route = route;
        req->eroute = eroute;
        req->autoFinalize = 1;
    }
}


static void closeEsp(HttpQueue *q)
{
    lock(esp);
    esp->inUse--;
    assert(esp->inUse >= 0);
    unlock(esp);
}



PUBLIC void espClearFlash(HttpConn *conn)
{
    EspReq      *req;

    req = conn->data;
    req->flash = 0;
}


static void setupFlash(HttpConn *conn)
{
    EspReq      *req;

    req = conn->data;
    if (httpGetSession(conn, 0)) {
        req->flash = httpGetSessionObj(conn, ESP_FLASH_VAR);
        req->lastFlash = 0;
        if (req->flash) {
            assert(req->flash->fn);
            httpSetSessionVar(conn, ESP_FLASH_VAR, "");
            req->lastFlash = mprCloneHash(req->flash);
        } else {
            req->flash = 0;
        }
    }
}


static void finalizeFlash(HttpConn *conn)
{
    EspReq  *req;
    MprKey  *kp, *lp;

    req = conn->data;
    if (req->flash) {
        assert(req->flash->fn);
        if (req->lastFlash) {
            assert(req->lastFlash->fn);
            for (ITERATE_KEYS(req->flash, kp)) {
                for (ITERATE_KEYS(req->lastFlash, lp)) {
                    if (smatch(kp->key, lp->key) && kp->data == lp->data) {
                        mprRemoveKey(req->flash, kp->key);
                    }
                }
            }
        }
        if (mprGetHashLength(req->flash) > 0) {
            /*  
                If the session does not exist, this will create one. However, must not have
                emitted the headers, otherwise cannot inform the client of the session cookie.
            */
            httpSetSessionObj(conn, ESP_FLASH_VAR, req->flash);
        }
    }
}


/*
    Start the request. At this stage, body data may not have been fully received unless 
    the request is a form (POST method and Content-Type is application/x-www-form-urlencoded).
    Forms are a special case and delay invoking the start callback until all body data is received.
 */
static void startEsp(HttpQueue *q)
{
    HttpConn    *conn;
    EspReq      *req;

    conn = q->conn;
    req = conn->data;

    if (req) {
        //  MOB - what if an error is set here?
        setupFlash(conn);
        if (!runAction(conn)) {
            return;
        }
        if (req->autoFinalize) {
            if (!conn->tx->responded) {
                espRenderView(conn, 0);
            }
            if (req->autoFinalize) {
                espFinalize(conn);
            }
        }
        if (espIsFinalized(conn)) {
            finalizeFlash(conn);
        }
    }
}


static int runAction(HttpConn *conn)
{
    HttpRx      *rx;
    HttpRoute   *route;
    EspRoute    *eroute;
    EspReq      *req;
    EspAction   action;
    char        *key, *controllerName, *actionName;
    int         updated;

    rx = conn->rx;
    req = conn->data;
    route = rx->route;
    eroute = req->eroute;
    assert(eroute);
    updated = 0;

    if (route->sourceName == 0 || *route->sourceName == '\0') {
        return 1;
    }
    /*
        Expand any form var $tokens. This permits ${controller} and user form data to be used in the controller name
     */
    if (schr(route->sourceName, '$')) {
        req->controllerName = stemplate(route->sourceName, rx->params);
    } else {
        req->controllerName = route->sourceName;
    }
    if (eroute->controllersDir) {
        req->controllerPath = mprJoinPath(eroute->controllersDir, req->controllerName);
    } else {
        req->controllerPath = mprJoinPath(route->dir, req->controllerName);
    }
    key = mprJoinPath(eroute->controllersDir, rx->target);

    if (eroute->appModuleName) {
        if (!loadApp(conn, &updated)) {
            return 0;
        }
#if !BIT_STATIC
    } else if (eroute->update || !mprLookupKey(esp->actions, key)) {
        char    *canonical;
        char    *source;
        int     recompile = 0;

        /* Trim the drive for VxWorks where simulated host drives only exist on the target */
        source = req->controllerPath;
#if VXWORKS
        source = mprTrimPathDrive(source);
#endif
        canonical = mprGetPortablePath(mprGetRelPath(source, route->dir));
        req->cacheName = mprGetMD5WithPrefix(canonical, slen(canonical), "controller_");
        req->module = mprNormalizePath(sfmt("%s/%s%s", eroute->cacheDir, req->cacheName, BIT_SHOBJ));

        if (!mprPathExists(req->controllerPath, R_OK)) {
            mprError("Cannot find controller %s", req->controllerPath);
            httpError(conn, HTTP_CODE_INTERNAL_SERVER_ERROR, "Cannot find controller to serve request");
            return 0;
        }
        lock(req->esp);
        if (eroute->update) {
            /*
                Test if the source has been updated. This will unload prior modules if stale
             */ 
            if (espModuleIsStale(req->controllerPath, req->module, &recompile)) {
                /*  WARNING: GC yield here */
                if (recompile && !espCompile(conn, req->controllerPath, req->module, req->cacheName, 0)) {
                    unlock(req->esp);
                    return 0;
                }
            }
        }
        if (mprLookupModule(req->controllerPath) == 0) {
            MprModule   *mp;
            req->entry = getControllerEntry(req->controllerName);
            if ((mp = mprCreateModule(req->controllerPath, req->module, req->entry, route)) == 0) {
                unlock(req->esp);
                httpMemoryError(conn);
                return 0;
            }
            mprSetThreadData(esp->local, conn);
            if (mprLoadModule(mp) < 0) {
                unlock(req->esp);
                httpError(conn, HTTP_CODE_INTERNAL_SERVER_ERROR, "Cannot load compiled esp module for %s", req->controllerPath);
                return 0;
            }
            updated = 1;
        }
        unlock(req->esp);
#endif
    }
    key = mprJoinPath(eroute->controllersDir, rx->target);
    if ((action = mprLookupKey(esp->actions, key)) == 0) {
        req->controllerPath = mprJoinPath(eroute->controllersDir, req->controllerName);
        key = sfmt("%s/missing", mprGetPathDir(req->controllerPath));
        if ((action = mprLookupKey(esp->actions, key)) == 0) {
            if (!viewExists(conn)) {
                if ((action = mprLookupKey(esp->actions, "missing")) == 0) {
                    httpError(conn, HTTP_CODE_INTERNAL_SERVER_ERROR, "Missing action for %s in %s", rx->target, req->controllerPath);
                    return 0;
                }
            }
        }
    }
    if (rx->flags & HTTP_POST) {
        if (!espCheckSecurityToken(conn)) {
            return 1;
        }
    }
    if (action) {
        controllerName = stok(sclone(rx->target), "-", &actionName);
        httpSetParam(conn, "controller", controllerName);
        httpSetParam(conn, "action", actionName);

        //  MOB - does this need a lock
        mprSetThreadData(esp->local, conn);
        if (eroute->controllerBase) {
            (eroute->controllerBase)(conn);
        }
        (action)(conn);
    }
    return 1;
}


PUBLIC void espRenderView(HttpConn *conn, cchar *name)
{
    HttpRx      *rx;
    EspRoute    *eroute;
    EspReq      *req;
    EspViewProc view;
    int         updated;
    
    rx = conn->rx;
    req = conn->data;
    eroute = req->eroute;
    updated = 0;
    
    if (name && *name) {
        req->view = mprJoinPath(eroute->viewsDir, name);
        req->source = mprJoinPathExt(req->view, ".esp");

    } else if (req->controllerName) {
        req->view = mprJoinPath(eroute->viewsDir, rx->target);
        req->source = mprJoinPathExt(req->view, ".esp");

    } else {
        httpMapFile(conn, rx->route);
        req->view = conn->tx->filename;
        req->source = req->view;
    }
    if (eroute->appModuleName) {
        if (!loadApp(conn, &updated)) {
            return;
        }
#if !BIT_STATIC
    } else if (eroute->update || !mprLookupKey(esp->views, mprGetPortablePath(req->source))) {
        cchar   *source;
        char    *canonical;
        int     recompile = 0;
        /* Trim the drive for VxWorks where simulated host drives only exist on the target */
        source = req->source;
#if VXWORKS
        source = mprTrimPathDrive(source);
#endif
        canonical = mprGetPortablePath(mprGetRelPath(source, req->route->dir));
        req->cacheName = mprGetMD5WithPrefix(canonical, slen(canonical), "view_");
        req->module = mprNormalizePath(sfmt("%s/%s%s", eroute->cacheDir, req->cacheName, BIT_SHOBJ));

        if (!mprPathExists(req->source, R_OK)) {
            mprLog(3, "Cannot find web page %s", req->source);
            httpError(conn, HTTP_CODE_NOT_FOUND, "Cannot find web page for %s", rx->uri);
            return;
        }
        lock(req->esp);
        if (eroute->update) {
            if (espModuleIsStale(req->source, req->module, &recompile)) {
                /* WARNING: this will allow GC */
                if (recompile && !espCompile(conn, req->source, req->module, req->cacheName, 1)) {
                    unlock(req->esp);
                    return;
                }
            }
        }
        if (mprLookupModule(req->source) == 0) {
            MprModule   *mp;
            req->entry = sfmt("esp_%s", req->cacheName);
            if ((mp = mprCreateModule(req->source, req->module, req->entry, req->route)) == 0) {
                unlock(req->esp);
                httpMemoryError(conn);
                return;
            }
            mprSetThreadData(esp->local, conn);
            if (mprLoadModule(mp) < 0) {
                unlock(req->esp);
                httpError(conn, HTTP_CODE_INTERNAL_SERVER_ERROR, "Cannot load compiled esp module for %s", req->source);
                return;
            }
        }
        unlock(req->esp);
#endif
    }
    if ((view = mprLookupKey(esp->views, mprGetPortablePath(req->source))) == 0) {
        httpError(conn, HTTP_CODE_NOT_FOUND, "Cannot find defined view for %s", req->view);
        return;
    }
    httpAddHeaderString(conn, "Content-Type", "text/html");
    mprSetThreadData(esp->local, conn);
    (view)(conn);
}


/************************************ Support *********************************/

#if !BIT_STATIC
static char *getControllerEntry(cchar *controllerName)
{
    char    *cp, *entry;

    entry = sfmt("esp_module_%s", mprTrimPathExt(mprGetPathBase(controllerName)));
    for (cp = entry; *cp; cp++) {
        if (!isalnum((uchar) *cp) && *cp != '_') {
            *cp = '_';
        }
    }
    return entry;
}
#endif


/*
    Load the (flat) app module. If modified, unload and reload if required.
 */
static int loadApp(HttpConn *conn, int *updated)
{
    MprModule   *mp;
    EspRoute    *eroute;
    EspReq      *req;
    char        *entry;

    req = conn->data;
    if (req->appLoaded) {
        return 1;
    }
    *updated = 0;
    eroute = req->eroute;
    if ((mp = mprLookupModule(eroute->appModuleName)) != 0) {
#if !BIT_STATIC
        if (eroute->update) {
            MprPath minfo;
            mprGetPathInfo(mp->path, &minfo);
            if (minfo.valid && mp->modified < minfo.mtime) {
                if (!espUnloadModule(eroute->appModuleName, 0)) {
                    mprError("Cannot unload module %s. Connections still open. Continue using old version.", 
                        eroute->appModuleName);
                    /* Cannot unload - so keep using old module */
                    return 1;
                }
                mp = 0;
            }
        }
#endif
    }
    if (mp == 0) {
        entry = sfmt("esp_app_%s", eroute->appModuleName);
        if ((mp = mprCreateModule(eroute->appModuleName, eroute->appModulePath, entry, req->route)) == 0) {
            httpError(conn, HTTP_CODE_INTERNAL_SERVER_ERROR, "Cannot find module %s", eroute->appModulePath);
            return 0;
        }
        mprSetThreadData(esp->local, conn);
        if (mprLoadModule(mp) < 0) {
            httpError(conn, HTTP_CODE_INTERNAL_SERVER_ERROR, "Cannot load compiled esp module for %s", eroute->appModuleName);
            return 0;
        }
        *updated = 1;
    }
    req->appLoaded = 1;
    return 1;
}


/*
    Test if the the required view page exists
 */
static bool viewExists(HttpConn *conn)
{
    HttpRx      *rx;
    EspRoute    *eroute;
    EspReq      *req;
    cchar       *source;
    
    rx = conn->rx;
    req = conn->data;
    eroute = req->eroute;
    
    source = mprJoinPathExt(mprJoinPath(eroute->viewsDir, rx->target), ".esp");
    return mprPathExists(source, R_OK);
}


/************************************ Esp Route *******************************/

static EspRoute *allocEspRoute(HttpRoute *route)
{
    EspRoute    *eroute;
    MprPath     info;
    cchar       *path;

    if ((eroute = mprAllocObj(EspRoute, espManageEspRoute)) == 0) {
        return 0;
    }
#if DEBUG_IDE
    path = mprGetAppDir();
#else
    path = httpGetRouteVar(route, "CACHE_DIR");
    if (!path || mprGetPathInfo(path, &info) != 0 || !info.isDir) {
        path = mprJoinPath(route->home, "cache");
    }
    if (mprGetPathInfo(path, &info) != 0 || !info.isDir) {
        path = route->home;
    }
#endif
    eroute->cacheDir = (char*) path;
    eroute->update = BIT_DEBUG;
    eroute->showErrors = BIT_DEBUG;
    eroute->keepSource = BIT_DEBUG;
    eroute->lifespan = 0;
    eroute->route = route;
    route->eroute = eroute;
    return eroute;
}


static EspRoute *cloneEspRoute(HttpRoute *route, EspRoute *parent)
{
    EspRoute      *eroute;
    
    assert(parent);
    assert(route);

    if ((eroute = mprAllocObj(EspRoute, espManageEspRoute)) == 0) {
        return 0;
    }
    eroute->searchPath = parent->searchPath;
    eroute->edi = parent->edi;
    eroute->controllerBase = parent->controllerBase;
    eroute->appModuleName = parent->appModuleName;
    eroute->appModulePath = parent->appModulePath;
    eroute->update = parent->update;
    eroute->keepSource = parent->keepSource;
    eroute->showErrors = parent->showErrors;
    eroute->lifespan = parent->lifespan;
    if (parent->compile) {
        eroute->compile = sclone(parent->compile);
    }
    if (parent->link) {
        eroute->link = sclone(parent->link);
    }
    if (parent->env) {
        eroute->env = mprCloneHash(parent->env);
    }
    eroute->cacheDir = parent->cacheDir;
    eroute->controllersDir = parent->controllersDir;
    eroute->dbDir = parent->dbDir;
    eroute->migrationsDir = parent->migrationsDir;
    eroute->layoutsDir = parent->layoutsDir;
    eroute->viewsDir = parent->viewsDir;
    eroute->staticDir = parent->staticDir;
    eroute->route = route;
    route->eroute = eroute;
    return eroute;
}


static void setMvcDirs(EspRoute *eroute, HttpRoute *route)
{
    char    *dir;

    dir = route->dir;

    eroute->cacheDir = mprJoinPath(dir, "cache");
    httpSetRouteVar(route, "CACHE_DIR", eroute->cacheDir);

    eroute->dbDir = mprJoinPath(dir, "db");
    httpSetRouteVar(route, "DB_DIR", eroute->dbDir);

    eroute->migrationsDir = mprJoinPath(dir, "db/migrations");
    httpSetRouteVar(route, "MIGRATIONS_DIR", eroute->migrationsDir);

    eroute->controllersDir = mprJoinPath(dir, "controllers");
    httpSetRouteVar(route, "CONTROLLERS_DIR", eroute->controllersDir);

    eroute->layoutsDir  = mprJoinPath(dir, "layouts");
    httpSetRouteVar(route, "LAYOUTS_DIR", eroute->layoutsDir);

    eroute->staticDir = mprJoinPath(dir, "static");
    httpSetRouteVar(route, "STATIC_DIR", eroute->staticDir);

    eroute->viewsDir = mprJoinPath(dir, "views");
    httpSetRouteVar(route, "VIEWS_DIR", eroute->viewsDir);
}


/*
    Manage all links for EspReq for the garbage collector
 */
static void manageReq(EspReq *req, int flags)
{
    if (flags & MPR_MANAGE_MARK) {
        mprMark(req->cacheName);
        mprMark(req->commandLine);
        mprMark(req->controllerName);
        mprMark(req->controllerPath);
        mprMark(req->entry);
        mprMark(req->eroute);
        mprMark(req->flash);
        mprMark(req->module);
        mprMark(req->record);
        mprMark(req->route);
        mprMark(req->source);
        mprMark(req->view);
    }
}


/*
    Manage all links for Esp for the garbage collector
 */
static void manageEsp(Esp *esp, int flags)
{
    if (flags & MPR_MANAGE_MARK) {
        mprMark(esp->actions);
        mprMark(esp->ediService);
        mprMark(esp->internalOptions);
        mprMark(esp->local);
        mprMark(esp->mutex);
        mprMark(esp->views);
    }
}


/*
    Get an EspRoute structure for a given route. Allocate or clone if required. It is expected that the caller will
    modify the EspRoute.
 */
static EspRoute *getEroute(HttpRoute *route)
{
    HttpRoute   *rp;

    if (route->eroute && ((EspRoute*) route->eroute)->route == route) {
        return route->eroute;
    }
    /*
        Lookup up the route chain for any configured EspRoutes
     */
    for (rp = route; rp; rp = rp->parent) {
        if (rp->eroute) {
            return cloneEspRoute(route, rp->eroute);
        }
    }
    return allocEspRoute(route);
}


static void setRouteDirs(MaState *state, cchar *kind)
{
    EspRoute    *eroute;

    if ((eroute = getEroute(state->route)) == 0) {
        return;
    }
    if (smatch(kind, "mvc") || smatch(kind, "mvc-simple") || smatch(kind, "restful")) {
        setMvcDirs(eroute, state->route);
    }
}


/*********************************** Directives *******************************/
/*
    EspApp Prefix [Dir [RouteSet [Database]]]
 */
static int espAppDirective(MaState *state, cchar *key, cchar *value)
{
    HttpRoute   *route;
    EspRoute    *eroute;
    char        *name, *prefix, *path, *routeSet, *database;
    bool        createRoute;

    createRoute = 0;

    if (!maTokenize(state, value, "%S ?S ?S ?S", &prefix, &path, &routeSet, &database)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    name = prefix;
    if (smatch(prefix, "/")) {
        prefix = 0;
    }
    if (smatch(prefix, state->route->prefix) && mprSamePath(state->route->dir, path)) {
        /* Can use existing route as it has the same prefix and documents directory */
        route = state->route;
    } else if ((route = httpCreateInheritedRoute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
        createRoute = 1;
    }
    if ((eroute = getEroute(route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    state = maPushState(state);
    state->route = route;

    if (prefix) {
        if (*prefix != '/') {
            mprError("Prefix name should start with a \"/\"");
            prefix = sjoin("/", prefix, NULL);
        }
        prefix = stemplate(prefix, route->vars);
    }
    if (createRoute) {
        if (prefix) {
            httpSetRouteName(route, prefix);
            httpSetRoutePattern(route, prefix, 0);
            httpSetRoutePrefix(route, prefix);
        }
        if (path) {
            httpSetRouteDir(route, path);
        }
    }
    httpAddRouteHandler(route, "espHandler", "");
    if (routeSet) {
        setRouteDirs(state, routeSet);
        httpAddRouteSet(state->route, routeSet);
    }
    if (database && espDbDirective(state, key, database) < 0) {
        maPopState(state);
        return MPR_ERR_BAD_STATE;
    }
#if BIT_STATIC
    setupEspApp(state->route, name, eroute->cacheDir);
#endif
    if (createRoute) {
        httpFinalizeRoute(route);
    }
    maPopState(state);
    return 0;
}


/*
    EspCompile template
 */
static int espCompileDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    eroute->compile = sclone(value);
    return 0;
}


/*
    EspDb provider://database
 */
static int espDbDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;
    char        *provider, *path;
    int         flags;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    //  MOB - is auto save really wanted?
    flags = EDI_CREATE | EDI_AUTO_SAVE;
    provider = stok(sclone(value), "://", &path);
    if (provider == 0 || path == 0) {
        mprError("Bad database spec '%s'. Use: provider://database", value);
        return MPR_ERR_CANT_OPEN;
    }
    path = mprJoinPath(eroute->dbDir, path);
    if ((eroute->edi = ediOpen(mprGetRelPath(path, NULL), provider, flags)) == 0) {
        if (!(state->flags & MA_PARSE_NON_SERVER)) {
            mprError("Cannot open database %s", path);
            return MPR_ERR_CANT_OPEN;
        }
    }
    //  MOB - who closes?
    return 0;
}


/*
    EspDir key path
 */
static int espDirDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;
    char        *name, *path;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    if (!maTokenize(state, value, "%S ?S", &name, &path)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    if (scmp(name, "mvc") == 0) {
        setMvcDirs(eroute, state->route);
    } else {
        path = stemplate(mprJoinPath(state->route->home, path), state->route->vars);
        if (scmp(name, "cache") == 0) {
            eroute->cacheDir = path;
        } else if (scmp(name, "controllers") == 0) {
            eroute->controllersDir = path;
        } else if (scmp(name, "db") == 0) {
            eroute->dbDir = path;
        } else if (scmp(name, "layouts") == 0) {
            eroute->layoutsDir = path;
        } else if (scmp(name, "migrations") == 0) {
            eroute->migrationsDir = path;
        } else if (scmp(name, "static") == 0) {
            eroute->staticDir = path;
        } else if (scmp(name, "views") == 0) {
            eroute->viewsDir = path;
        }
        httpSetRouteVar(state->route, name, path);
    }
    return 0;
}


/*
    Define Visual Studio environment if not already present
 */
static void defineVisualStudioEnv(MaState *state)
{
    MaAppweb    *appweb;
    int         is64BitSystem;

    appweb = MPR->appwebService;

    if (scontains(getenv("LIB"), "Visual Studio") &&
        scontains(getenv("INCLUDE"), "Visual Studio") &&
        scontains(getenv("PATH"), "Visual Studio")) {
        return;
    }
    if (scontains(appweb->platform, "-x64-")) {
        is64BitSystem = smatch(getenv("PROCESSOR_ARCHITECTURE"), "AMD64") || getenv("PROCESSOR_ARCHITEW6432");
        espEnvDirective(state, "EspEnv", "LIB \"${WINSDK}\\LIB\\win8\\um\\x64;${WINSDK}\\LIB\\x64;${VS}\\VC\\lib\\amd64\"");
        if (is64BitSystem) {
            espEnvDirective(state, "EspEnv", "PATH \"${VS}\\Common7\\IDE;${VS}\\VC\\bin\\amd64;${VS}\\Common7\\Tools;${VS}\\SDK\\v3.5\\bin;${VS}\\VC\\VCPackages;${WINSDK}\\bin\\x64\"");

        } else {
            /* Cross building on x86 for 64-bit */
            espEnvDirective(state, "EspEnv", "PATH \"${VS}\\Common7\\IDE;${VS}\\VC\\bin\\x86_amd64;${VS}\\Common7\\Tools;${VS}\\SDK\\v3.5\\bin;${VS}\\VC\\VCPackages;${WINSDK}\\bin\\x86\"");
        }

    } else if (scontains(appweb->platform, "-arm-")) {
        /* Cross building on x86 for arm. No winsdk 7 support for arm */
        espEnvDirective(state, "EspEnv", "LIB \"${WINSDK}\\LIB\\win8\\um\\arm;${VS}\\VC\\lib\\arm\"");
        espEnvDirective(state, "EspEnv", "PATH \"${VS}\\Common7\\IDE;${VS}\\VC\\bin\\x86_arm;${VS}\\Common7\\Tools;${VS}\\SDK\\v3.5\\bin;${VS}\\VC\\VCPackages;${WINSDK}\\bin\\arm\"");

    } else {
        /* Building for X86 */
        espEnvDirective(state, "EspEnv", "LIB \"${WINSDK}\\LIB\\win8\\um\\x86;${WINSDK}\\LIB\\x86;${WINSDK}\\LIB;${VS}\\VC\\lib\"");
        espEnvDirective(state, "EspEnv", "PATH \"${VS}\\Common7\\IDE;${VS}\\VC\\bin;${VS}\\Common7\\Tools;${VS}\\SDK\\v3.5\\bin;${VS}\\VC\\VCPackages;${WINSDK}\\bin\"");
    }
    espEnvDirective(state, "EspEnv", "INCLUDE \"${VS}\\VC\\INCLUDE;${WINSDK}\\include;${WINSDK}\\include\\um;${WINSDK}\\include\\shared\"");
}


/*
    EspEnv var string
    This defines an environment variable setting. It is defined only when commands for this route are executed.
 */
static int espEnvDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;
    char        *ekey, *evalue;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    if (!maTokenize(state, value, "%S ?S", &ekey, &evalue)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    if (eroute->env == 0) {
        eroute->env = mprCreateHash(-1, 0);
    }
    evalue = espExpandCommand(eroute, evalue, "", "");
    if (scaselessmatch(ekey, "VisualStudio")) {
        defineVisualStudioEnv(state);
    } else {
        mprAddKey(eroute->env, ekey, evalue);
    }
    if (scaselessmatch(ekey, "PATH")) {
        if (eroute->searchPath) {
            eroute->searchPath = sclone(evalue);
        } else {
            eroute->searchPath = sjoin(eroute->searchPath, MPR_SEARCH_SEP, evalue, NULL);
        }
    }
    return 0;
}


/*
    EspKeepSource on|off
 */
static int espKeepSourceDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;
    bool        on;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    if (!maTokenize(state, value, "%B", &on)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    eroute->keepSource = on;
    return 0;
}


/*
    EspLink template
 */
static int espLinkDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    eroute->link = sclone(value);
    return 0;
}


/*
    Inner code for EspLoad
 */
static int setupEspApp(HttpRoute *route, cchar *name, cchar *path)
{
    EspRoute    *eroute, *ep;
    HttpRoute   *rp;
    HttpHost    *host;
    int         next;

    if ((eroute = getEroute(route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    eroute->appModuleName = sclone(name);
    eroute->appModulePath = sclone(path);

    host = httpGetDefaultHost();
    for (ITERATE_ITEMS(host->routes, rp, next)) {
        if (rp->eroute && rp->parent == route) {
            ep = rp->eroute;
            ep->appModuleName = eroute->appModuleName;
            ep->appModulePath = eroute->appModulePath;
        }
    }
    return 0;
}


/*
    Initialize and load a statically linked ESP module
 */
PUBLIC int espStaticInitialize(EspModuleEntry entry, cchar *appName, cchar *routeName)
{
    HttpRoute   *route;
    EspRoute    *eroute;

    if ((route = httpLookupRoute(NULL, routeName)) == 0) {
        mprError("Cannot find route %s", routeName);
        return MPR_ERR_CANT_ACCESS;
    }
    if ((eroute = getEroute(route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    if (setupEspApp(route, appName, eroute->cacheDir) < 0) {
        return MPR_ERR_MEMORY;
    }
    return (entry)(route, NULL);
}


/*
    EspLoad name path
 */
static int espLoadDirective(MaState *state, cchar *key, cchar *value)
{
    char    *name, *path;

    if (!maTokenize(state, value, "%S %P", &name, &path)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    setupEspApp(state->route, name, path);
    return 0;
}


/*
    EspResource [resource ...]
 */
static int espResourceDirective(MaState *state, cchar *key, cchar *value)
{
    char    *name, *next;

    if (value == 0 || *value == '\0') {
        httpAddResource(state->route, "{controller}");
    } else {
        name = stok(sclone(value), ", \t\r\n", &next);
        while (name) {
            httpAddResource(state->route, name);
            name = stok(NULL, ", \t\r\n", &next);
        }
    }
    return 0;
}


/*
    EspResourceGroup [resource ...]
 */
static int espResourceGroupDirective(MaState *state, cchar *key, cchar *value)
{
    char    *name, *next;

    if (value == 0 || *value == '\0') {
        httpAddResourceGroup(state->route, "{controller}");
    } else {
        name = stok(sclone(value), ", \t\r\n", &next);
        while (name) {
            httpAddResourceGroup(state->route, name);
            name = stok(NULL, ", \t\r\n", &next);
        }
    }
    return 0;
}


/*
    EspRoute name methods pattern target source
 */
static int espRouteDirective(MaState *state, cchar *key, cchar *value)
{
    char    *name, *methods, *pattern, *target, *source;

    if (!maTokenize(state, value, "%S %S %S %S ?S", &name, &methods, &pattern, &target, &source)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    target = stemplate(target, state->route->vars);
    httpDefineRoute(state->route, name, methods, pattern, target, source);
    return 0;
}


PUBLIC int espBindProc(HttpRoute *parent, cchar *pattern, void *proc)
{
    EspRoute    *eroute;
    HttpRoute   *route;

    if ((route = httpDefineRoute(parent, pattern, "ALL", pattern, "$&", "unused")) == 0) {
        return MPR_ERR_CANT_CREATE;
    }
    httpSetRouteHandler(route, "espHandler");

    if ((eroute = getEroute(route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    eroute->update = 0;
    espDefineAction(route, pattern, proc);
    return 0;
}


/*
    EspRouteSet kind
 */
static int espRouteSetDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;
    char        *kind;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    if (!maTokenize(state, value, "%S", &kind)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    httpAddRouteSet(state->route, kind);
    return 0;
}


/*
    EspShowErrors on|off
 */
static int espShowErrorsDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;
    bool        on;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    if (!maTokenize(state, value, "%B", &on)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    eroute->showErrors = on;
    return 0;
}


/*
    EspUpdate on|off
 */
static int espUpdateDirective(MaState *state, cchar *key, cchar *value)
{
    EspRoute    *eroute;
    bool        on;

    if ((eroute = getEroute(state->route)) == 0) {
        return MPR_ERR_MEMORY;
    }
    if (!maTokenize(state, value, "%B", &on)) {
        return MPR_ERR_BAD_SYNTAX;
    }
    eroute->update = on;
    return 0;
}


/************************************ Init ************************************/
/*
    Loadable module configuration
 */
PUBLIC int maEspHandlerInit(Http *http, MprModule *module)
{
    HttpStage   *handler;
    MaAppweb    *appweb;

    appweb = httpGetContext(http);

    if ((handler = httpCreateHandler(http, "espHandler", module)) == 0) {
        return MPR_ERR_CANT_CREATE;
    }
    http->espHandler = handler;
    handler->open = openEsp; 
    handler->close = closeEsp; 
    handler->start = startEsp; 
    if ((esp = mprAllocObj(Esp, manageEsp)) == 0) {
        return MPR_ERR_MEMORY;
    }
    handler->stageData = esp;
    MPR->espService = esp;
    esp->mutex = mprCreateLock();
    esp->local = mprCreateThreadLocal();
    if (module) {
        mprSetModuleFinalizer(module, unloadEsp);
    }
    if ((esp->internalOptions = mprCreateHash(-1, MPR_HASH_STATIC_VALUES)) == 0) {
        return 0;
    }
    espInitHtmlOptions(esp);

    if ((esp->views = mprCreateHash(-1, MPR_HASH_STATIC_VALUES)) == 0) {
        return 0;
    }
    if ((esp->actions = mprCreateHash(-1, MPR_HASH_STATIC_VALUES)) == 0) {
        return 0;
    }

    /*
        Add configuration file directives
     */
    maAddDirective(appweb, "EspApp", espAppDirective);
    maAddDirective(appweb, "EspCompile", espCompileDirective);
    maAddDirective(appweb, "EspDb", espDbDirective);
    maAddDirective(appweb, "EspDir", espDirDirective);
    maAddDirective(appweb, "EspEnv", espEnvDirective);
    maAddDirective(appweb, "EspKeepSource", espKeepSourceDirective);
    maAddDirective(appweb, "EspLink", espLinkDirective);
    maAddDirective(appweb, "EspLoad", espLoadDirective);
    maAddDirective(appweb, "EspResource", espResourceDirective);
    maAddDirective(appweb, "EspResourceGroup", espResourceGroupDirective);
    maAddDirective(appweb, "EspRoute", espRouteDirective);
    maAddDirective(appweb, "EspRouteSet", espRouteSetDirective);
    maAddDirective(appweb, "EspShowErrors", espShowErrorsDirective);
    maAddDirective(appweb, "EspUpdate", espUpdateDirective);

    if ((esp->ediService = ediCreateService()) == 0) {
        return 0;
    }
#if BIT_PACK_MDB
    /* Memory database */
    mdbInit();
#endif
#if BIT_PACK_SDB
    /* Sqlite */
    sdbInit();
#endif
    return 0;
}


static int unloadEsp(MprModule *mp)
{
    HttpStage   *stage;

    if (esp->inUse) {
       return MPR_ERR_BUSY;
    }
    if ((stage = httpLookupStage(MPR->httpService, mp->name)) != 0) {
        stage->flags |= HTTP_STAGE_UNLOADED;
    }
    return 0;
}

#else /* BIT_PACK_ESP */

PUBLIC int maEspHandlerInit(Http *http, MprModule *mp)
{
    mprNop(0);
    return 0;
}

#endif /* BIT_PACK_ESP */
/*
    @copy   default

    Copyright (c) Embedthis Software LLC, 2003-2013. All Rights Reserved.

    This software is distributed under commercial and open source licenses.
    You may use the Embedthis Open Source license or you may acquire a 
    commercial license from Embedthis Software. You agree to be fully bound
    by the terms of either license. Consult the LICENSE.md distributed with
    this software for full details and other copyrights.

    Local variables:
    tab-width: 4
    c-basic-offset: 4
    End:
    vim: sw=4 ts=4 expandtab

    @end
 */
