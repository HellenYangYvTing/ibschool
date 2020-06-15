/*
    http.h -- Header for the Embedthis Http Library.

    Copyright (c) All Rights Reserved. See copyright notice at the bottom of the file.
 */

#ifndef _h_HTTP
#define _h_HTTP 1

/********************************* Includes ***********************************/

#include    "mpr.h"

/****************************** Forward Declarations **************************/

#ifdef __cplusplus
extern "C" {
#endif

#if !DOXYGEN
struct Http;
struct HttpAuth;
struct HttpConn;
struct HttpEndpoint;
struct HttpHost;
struct HttpLimits;
struct HttpPacket;
struct HttpQueue;
struct HttpRoute;
struct HttpRx;
struct HttpSession;
struct HttpStage;
struct HttpTx;
struct HttpUri;
struct HttpUser;
struct HttpWebSocket;
#endif

/********************************** Tunables **********************************/

#ifndef BIT_HTTP_PORT
    #define BIT_HTTP_PORT           80
#endif
#ifndef BIT_HTTP_SOFTWARE
    #define BIT_HTTP_SOFTWARE       "Embedthis-http"    /**< Default Http protocol name used in Http Server header */
#endif
#ifndef BIT_MAX_URI
    #define BIT_MAX_URI             1024                /**< Reasonable URI size */
#endif
#ifndef BIT_MAX_IOVEC
    #define BIT_MAX_IOVEC           16                  /**< Number of fragments in a single socket write */
#endif
#ifndef BIT_MAX_ROUTE_MATCHES
    #define BIT_MAX_ROUTE_MATCHES   32                  /**< Maximum number of submatches in routes */
#endif
#ifndef BIT_MAX_CLIENTS_HASH
    #define BIT_MAX_CLIENTS_HASH    131                 /**< Hash table for client IP addresses */
#endif
#ifndef  BIT_MAX_CACHE_ITEM
    #define BIT_MAX_CACHE_ITEM      (256 * 1024)        /**< Maximum cachable item size */
#endif
#ifndef BIT_MAX_CHUNK
    #define BIT_MAX_CHUNK           (8 * 1024)          /**< Maximum chunk size for transfer chunk encoding */
#endif
#ifndef BIT_MAX_CLIENTS
    #define BIT_MAX_CLIENTS         32                  /**< Maximum concurrent client endpoints */
#endif
#ifndef BIT_MAX_HEADERS
    #define BIT_MAX_HEADERS         8192                /**< Maximum size of the headers */
#endif
#ifndef BIT_MAX_KEEP_ALIVE
    #define BIT_MAX_KEEP_ALIVE      200                 /**< Maximum requests per connection */
#endif
#ifndef BIT_MAX_NUM_HEADERS
    #define BIT_MAX_NUM_HEADERS     30                  /**< Maximum number of header lines */
#endif
#ifndef BIT_MAX_RECEIVE_BODY
    #define BIT_MAX_RECEIVE_BODY    (128 * 1024 * 1024) /**< Maximum incoming body size */
#endif
#ifndef BIT_MAX_RECEIVE_FORM
    #define BIT_MAX_RECEIVE_FORM    (1024 * 1024)       /**< Maximum incoming form size */
#endif
#ifndef BIT_MAX_REQUESTS
    #define BIT_MAX_REQUESTS        50                  /**< Maximum concurrent requests */
#endif
#ifndef BIT_MAX_REQUESTS_PER_CLIENT
    #define BIT_MAX_REQUESTS_PER_CLIENT 20              /**< Maximum concurrent requests */
#endif
#ifndef BIT_MAX_REWRITE
    #define BIT_MAX_REWRITE         20                  /**< Maximum URI rewrites */
#endif
#ifndef BIT_MAX_SESSIONS
    #define BIT_MAX_SESSIONS        100                 /**< Maximum concurrent sessions */
#endif
#ifndef BIT_MAX_QBUFFER
    #define BIT_MAX_QBUFFER         (32 * 1024)         /**< Maximum buffer for any pipeline queue */
#endif
#ifndef BIT_MAX_TX_BODY
    #define BIT_MAX_TX_BODY         (INT_MAX)           /**< Maximum buffer for response data */
#endif
#ifndef BIT_MAX_UPLOAD
    #define BIT_MAX_UPLOAD          (INT_MAX)
#endif
#ifndef BIT_MAX_WSS_FRAME
    #define BIT_MAX_WSS_FRAME       (4 * 1024)          /**< Default max WebSockets message frame size */
#endif
#ifndef BIT_MAX_WSS_PACKET
    #define BIT_MAX_WSS_PACKET      (8 * 1024)          /**< Default size to provide to application in one packet */
#endif
#ifndef BIT_MAX_WSS_SOCKETS
    #define BIT_MAX_WSS_SOCKETS     200                 /**< Default max WebSockets */
#endif

#ifndef BIT_MAX_CACHE_DURATION
    #define BIT_MAX_CACHE_DURATION  (86400 * 1000)      /**< Default cache lifespan to 1 day */
#endif
#ifndef BIT_MAX_INACTIVITY_DURATION
    #define BIT_MAX_INACTIVITY_DURATION (60  * 1000)    /**< Default keep connection alive between requests timeout */
#endif
#ifndef BIT_MAX_PARSE_DURATION
    #define BIT_MAX_PARSE_DURATION  (30  * 1000)        /**< Default request parse header timeout */
#endif
#ifndef BIT_MAX_REQUEST_DURATION
    #define BIT_MAX_REQUEST_DURATION MAXINT             /**< Default request timeout (unlimited) */
#endif
#ifndef BIT_MAX_SESSION_DURATION
    #define BIT_MAX_SESSION_DURATION (3600 * 1000)      /**< Default session timeout (one hour) */
#endif
#ifndef BIT_MAX_PING_DURATION
    #define BIT_MAX_PING_DURATION (30 * 1000)           /**< WSS ping defeat Keep-Alive timeouts (30 sec) */
#endif

#ifndef BIT_HTTP_LOG
    #define BIT_HTTP_LOG_FORMAT     "%h %l %u %t \"%r\" %>s %b %n"
#endif

#define HTTP_RETRIES                3                   /**< Default number of retries for client requests */
#define HTTP_DATE_FORMAT            "%a, %d %b %Y %T GMT"
#define HTTP_MAX_SECRET             16                  /**< Size of secret data for auth */
#define HTTP_MAX_WSS_MESSAGE        (2147483647)        /**< Default max WebSockets message size (2GB) */
#define HTTP_SMALL_HASH_SIZE        31                  /* Small hash (less than the alphabet) */
#define HTTP_TIMER_PERIOD           1000                /**< HttpTimer checks ever 1 second */

#define HTTP_PACKET_ALIGN(x)        (((x) + 0x3FF) & ~0x3FF)

/********************************** Defines ***********************************/
/*
    Standard HTTP/1.1 status codes
 */
#define HTTP_CODE_CONTINUE                  100     /**< Continue with request, only partial content transmitted */
#define HTTP_CODE_SWITCHING                 101     /**< Switching protocols */
#define HTTP_CODE_OK                        200     /**< The request completed successfully */
#define HTTP_CODE_CREATED                   201     /**< The request has completed and a new resource was created */
#define HTTP_CODE_ACCEPTED                  202     /**< The request has been accepted and processing is continuing */
#define HTTP_CODE_NOT_AUTHORITATIVE         203     /**< The request has completed but content may be from another source */
#define HTTP_CODE_NO_CONTENT                204     /**< The request has completed and there is no response to send */
#define HTTP_CODE_RESET                     205     /**< The request has completed with no content. Client must reset view */
#define HTTP_CODE_PARTIAL                   206     /**< The request has completed and is returning partial content */
#define HTTP_CODE_MOVED_PERMANENTLY         301     /**< The requested URI has moved permanently to a new location */
#define HTTP_CODE_MOVED_TEMPORARILY         302     /**< The URI has moved temporarily to a new location */
#define HTTP_CODE_SEE_OTHER                 303     /**< The requested URI can be found at another URI location */
#define HTTP_CODE_NOT_MODIFIED              304     /**< The requested resource has changed since the last request */
#define HTTP_CODE_USE_PROXY                 305     /**< The requested resource must be accessed via the location proxy */
#define HTTP_CODE_TEMPORARY_REDIRECT        307     /**< The request should be repeated at another URI location */
#define HTTP_CODE_BAD_REQUEST               400     /**< The request is malformed */
#define HTTP_CODE_UNAUTHORIZED              401     /**< Authentication for the request has failed */
#define HTTP_CODE_PAYMENT_REQUIRED          402     /**< Reserved for future use */
#define HTTP_CODE_FORBIDDEN                 403     /**< The request was legal, but the server refuses to process */
#define HTTP_CODE_NOT_FOUND                 404     /**< The requested resource was not found */
#define HTTP_CODE_BAD_METHOD                405     /**< The request HTTP method was not supported by the resource */
#define HTTP_CODE_NOT_ACCEPTABLE            406     /**< The requested resource cannot generate the required content */
#define HTTP_CODE_REQUEST_TIMEOUT           408     /**< The server timed out waiting for the request to complete */
#define HTTP_CODE_CONFLICT                  409     /**< The request had a conflict in the request headers and URI */
#define HTTP_CODE_GONE                      410     /**< The requested resource is no longer available*/
#define HTTP_CODE_LENGTH_REQUIRED           411     /**< The request did not specify a required content length*/
#define HTTP_CODE_PRECOND_FAILED            412     /**< The server cannot satisfy one of the request preconditions */
#define HTTP_CODE_REQUEST_TOO_LARGE         413     /**< The request is too large for the server to process */
#define HTTP_CODE_REQUEST_URL_TOO_LARGE     414     /**< The request URI is too long for the server to process */
#define HTTP_CODE_UNSUPPORTED_MEDIA_TYPE    415     /**< The request media type is not supported by the server or resource */
#define HTTP_CODE_RANGE_NOT_SATISFIABLE     416     /**< The request content range does not exist for the resource */
#define HTTP_CODE_EXPECTATION_FAILED        417     /**< The server cannot satisfy the Expect header requirements */
#define HTTP_CODE_NO_RESPONSE               444     /**< The connection was closed with no response to the client */
#define HTTP_CODE_INTERNAL_SERVER_ERROR     500     /**< Server processing or configuration error. No response generated */
#define HTTP_CODE_NOT_IMPLEMENTED           501     /**< The server does not recognize the request or method */
#define HTTP_CODE_BAD_GATEWAY               502     /**< The server cannot act as a gateway for the given request */
#define HTTP_CODE_SERVICE_UNAVAILABLE       503     /**< The server is currently unavailable or overloaded */
#define HTTP_CODE_GATEWAY_TIMEOUT           504     /**< The server gateway timed out waiting for the upstream server */
#define HTTP_CODE_BAD_VERSION               505     /**< The server does not support the HTTP protocol version */
#define HTTP_CODE_INSUFFICIENT_STORAGE      507     /**< The server has insufficient storage to complete the request */

/*
    Proprietary HTTP status codes
 */
#define HTTP_CODE_START_LOCAL_ERRORS        550
#define HTTP_CODE_COMMS_ERROR               550     /**< The server had a communicationss error responding to the client */
#define HTTP_CODE_BAD_HANDSHAKE             551     /**< The server handsake response is unacceptable */

/*
    Flags that can be ored into the status code
 */
#define HTTP_CODE_MASK                      0xFFFF
#define HTTP_ABORT                          0x10000 /* Abort the request and connection */
#define HTTP_CLOSE                          0x20000 /* Close the conn at the completion of the request */

/**
    Connection Http state change notification callback
    @description The notifier callback is invoked for state changes and I/O events. A user notifier function can
        respond to these events with any desired custom code.
        There are four valid event types:
        <ul>
            <li>HTTP_EVENT_STATE. The connection object has changed state. See conn->state.</li>
            <li>HTTP_EVENT_READABLE. The input queue has I/O to read. See conn->readq.
                Use #httpRead to read the data. For WebSockets, use #httpGetPacket.</li>
            <li>HTTP_EVENT_WRITABLE. The output queue is now writable.</li>
            <li>HTTP_EVENT_ERROR. The connection or request has an error. </li>
        </ul>
    @param conn HttpConn connection object created via #httpCreateConn
    @param event Http state
    @param arg Per-event information
    @ingroup HttpConn
    @stability Stable
 */
typedef void (*HttpNotifier)(struct HttpConn *conn, int event, int arg);

/**
    Set environment vars callback. Invoked per request to permit custom form var definition
    @ingroup HttpConn
    @stability Stable
 */
typedef void (*HttpEnvCallback)(struct HttpConn *conn);

/**
    Listen callback. Invoked after listening on a socket endpoint
    @return "Zero" if the listening endpoint can be opened for service. Otherwise, return a negative MPR error code.
    @ingroup HttpConn
    @stability Stable
 */
typedef int (*HttpListenCallback)(struct HttpEndpoint *endpoint);

/** 
    Define an callback for IO events on this connection.
    @description The event callback will be invoked in response to I/O events.
    @param conn HttpConn connection object created via #httpCreateConn
    @param fn Callback function. 
    @param arg Data argument to provide to the callback function.
    @return The redirected URI string to use.
    @ingroup HttpConn
    @stability Stable
 */
typedef cchar *(*HttpRedirectCallback)(struct HttpConn *conn, int *code, struct HttpUri *uri);

/**
    Timeout callback
    @description The timeout callback for the request inactivity and duration timeouts
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpConn
    @stability Stable
  */
typedef void (*HttpTimeoutCallback)(struct HttpConn *conn);

/**
    Set the fork callback.
    @param http Http service object.
    @param proc Fork callback procedure
    @param arg Argument to supply when the callback is invoked.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetForkCallback(struct Http *http, MprForkCallback proc, void *arg);

/************************************ Http **********************************/
/** 
    Http service object
    @description The Http service is managed by a single service object.
    @defgroup Http Http
    @see Http HttpConn HttpEndpoint gettGetDateString httpConfigurenamedVirtualEndpoint httpCreate httpCreateSecret 
        httpDestroy httpGetContext httpGetDateString httpLookupEndpoint httpLookupStatus httpLooupHost 
        httpSetContext httpSetDefaultClientHost httpSetDefaultClientPort httpSetDefaultPort httpSetForkCallback 
        httpSetProxy httpSetSoftware 
    @stability Internal
 */
typedef struct Http {
    MprList         *endpoints;             /**< Currently configured listening endpoints */
    MprList         *hosts;                 /**< List of host objects */
    MprList         *connections;           /**< Currently open connection requests */
    MprHash         *stages;                /**< Possible stages in connection pipelines */
    MprCache        *sessionCache;          /**< Session state cache */
    MprHash         *statusCodes;           /**< Http status codes */

    MprHash         *routeTargets;          /**< Http route target functions */
    MprHash         *routeConditions;       /**< Http route condition functions */
    MprHash         *routeUpdates;          /**< Http route update functions */

    MprHash         *authTypes;             /**< Available authentication protocol types */
    MprHash         *authStores;            /**< Available password stores */

    /*  
        Some standard pipeline stages
     */
    struct HttpStage *actionHandler;        /**< Action handler */
    struct HttpStage *cacheFilter;          /**< Cache filter */
    struct HttpStage *cacheHandler;         /**< Cache filter */
    struct HttpStage *chunkFilter;          /**< Chunked transfer encoding filter */
    struct HttpStage *cgiHandler;           /**< CGI handler */
    struct HttpStage *cgiConnector;         /**< CGI connector */
    struct HttpStage *clientHandler;        /**< Client-side handler (dummy) */
    struct HttpStage *dirHandler;           /**< Directory listing handler */
    struct HttpStage *egiHandler;           /**< Embedded Gateway Interface (EGI) handler */
    struct HttpStage *ejsHandler;           /**< Ejscript Web Framework handler */
    struct HttpStage *espHandler;           /**< ESP Web Framework handler */
    struct HttpStage *fileHandler;          /**< Static file handler */
    struct HttpStage *netConnector;         /**< Default network connector */
    struct HttpStage *passHandler;          /**< Pass through handler */
    struct HttpStage *phpHandler;           /**< PHP through handler */
    struct HttpStage *rangeFilter;          /**< Ranged requests filter */
    struct HttpStage *sendConnector;        /**< Optimized sendfile connector */
    struct HttpStage *uploadFilter;         /**< Upload filter */
    struct HttpStage *webSocketFilter;      /**< WebSocket filter */

    struct HttpLimits *clientLimits;        /**< Client resource limits */
    struct HttpLimits *serverLimits;        /**< Server resource limits */
    struct HttpRoute *clientRoute;          /**< Default route for clients */

    MprEvent        *timer;                 /**< Admin service timer */
    MprEvent        *timestamp;             /**< Timestamp timer */
    MprTime         booted;                 /**< Time the server started */
    MprTicks        now;                    /**< When was the currentDate last computed */
    MprMutex        *mutex;

    char            *software;              /**< Software name and version */
    void            *forkData;

    int             nextAuth;               /**< Auth object version vector */
    int             activeProcesses;        /**< Count of active external processes */
    int             activeSessions;         /**< Count of active sessions */
    uint64          totalConnections;       /**< Total connections accepted */
    uint64          totalRequests;          /**< Total requests served */

    int             activeVMs;              /**< Number of ejs VMs */
    int             flags;                  /**< Open flags */
    int             underAttack;            /**< Under DOS attack */
    void            *context;               /**< Embedding context */
    MprTicks        currentTime;            /**< When currentDate was last calculated (ticks) */
    char            *currentDate;           /**< Date string for HTTP response headers */
    char            *secret;                /**< Random bytes for authentication */

    char            *defaultClientHost;     /**< Default ip address */
    int             defaultClientPort;      /**< Default port */

    char            *protocol;              /**< HTTP/1.0 or HTTP/1.1 */
    char            *proxyHost;             /**< Proxy ip address */
    int             proxyPort;              /**< Proxy port */

    /*
        Callbacks
     */
    HttpEnvCallback     envCallback;        /**< SetEnv callback */
    MprForkCallback     forkCallback;       /**< Callback in child after fork() */
    HttpListenCallback  listenCallback;     /**< Invoked when creating listeners */
    HttpRedirectCallback redirectCallback;  /**< Redirect callback */
} Http;


/*
    Flags for httpCreate
 */
#define HTTP_CLIENT_SIDE    0x1             /**< Initialize the client-side support */
#define HTTP_SERVER_SIDE    0x2             /**< Initialize the server-side support */
#define HTTP_UTILITY        0x4             /**< Http engine for utilities (esp) */

/**
    Create a Http service object
    @description Create a http service object. One http service object should be created per application.
    @param flags Set to zero to initialize bo Initialize the client-side support only. 
    @return The http service object.
    @ingroup Http
    @stability Stable
 */
PUBLIC Http *httpCreate(int flags);

/**
    Configure endpoints with named virtual hosts
    @param http Http service object.
    @param ip IP address for the named virtual host
    @param port address for the named virtual host
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC int httpConfigureNamedVirtualEndpoints(Http *http, cchar *ip, int port);

/**  
    Create the Http secret data for crypto
    @description Create http secret data that is used to seed SSL-based communications.
    @param http Http service object.
    @return "Zero" if successful, otherwise a negative MPR error code
    @ingroup Http
    @stability Stable
 */
PUBLIC int httpCreateSecret(Http *http);

/**
    Destroy the Http service object
    @param http Http service object.
    @ingroup Http
    @stability Stable
 */
PUBLIC void httpDestroy(Http *http);

/**
    Get the http context object
    @param http Http service object.
    @return The http context object defined via httpSetContext
    @ingroup Http
    @stability Stable
 */
PUBLIC void *httpGetContext(Http *http);

/**
    Get the time as an ISO date string
    @param sbuf Optional path buffer. If supplied, the modified time of the path is used. If NULL, then the current
        time is used.
    @return RFC822 formatted date string. 
    @ingroup Http
    @stability Stable
 */
PUBLIC char *httpGetDateString(MprPath *sbuf);

/**
    Set the http context object
    @param http Http object created via #httpCreate
    @param context New context object
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetContext(Http *http, void *context);

/** 
    Define a default client host
    @description Define a default host to use for client connections if the URI does not specify a host
    @param http Http object created via #httpCreateConn
    @param host Host or IP address
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetDefaultClientHost(Http *http, cchar *host);

/** 
    Define a default client port
    @description Define a default port to use for client connections if the URI does not define a port
    @param http Http object created via #httpCreateConn
    @param port Integer port number
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetDefaultClientPort(Http *http, int port);

/** 
    Define a Http proxy host to use for all client connect requests.
    @description Define a http proxy host to communicate via when accessing the net.
    @param http Http object created via #httpCreate
    @param host Proxy host name or IP address
    @param port Proxy host port number.
    @ingroup Http
    @stability Stable
 */
PUBLIC void httpSetProxy(Http *http, cchar *host, int port);

/**
    Lookup a Http status code
    @description Lookup the code and return the corresponding text message briefly expaining the status.
    @param http Http object created via #httpCreate
    @param status Http status code
    @return Text message corresponding to the status code
    @ingroup Http
    @stability Stable
 */
PUBLIC cchar *httpLookupStatus(Http *http, int status);

/**
    Lookup a host by name
    @param http Http object created via #httpCreate
    @param name The name of the host to find
    @return The corresponding host object
    @ingroup Http
    @stability Stable
 */
PUBLIC struct HttpHost *httpLookupHost(Http *http, cchar *name);

/**
    Lookup a listening endpoint
    @param http Http object created via #httpCreate
    @param ip Listening IP address to look for
    @param port Listening port number
    @return HttpEndpoint object
    @ingroup Http
    @stability Stable
 */
PUBLIC struct HttpEndpoint *httpLookupEndpoint(Http *http, cchar *ip, int port);

/**
    Set the software description
    @param http Http object created via #httpCreate
    @param description String describing the Http software. By default, this is set to HTTP_NAME.
    @ingroup Http
    @stability Stable
 */
PUBLIC void httpSetSoftware(Http *http, cchar *description);

/* Internal APIs */
PUBLIC void httpAddConn(Http *http, struct HttpConn *conn);
PUBLIC struct HttpEndpoint *httpGetFirstEndpoint(Http *http);
PUBLIC void httpRemoveConn(Http *http, struct HttpConn *conn);
PUBLIC void httpAddEndpoint(Http *http, struct HttpEndpoint *endpoint);
PUBLIC void httpRemoveEndpoint(Http *http, struct HttpEndpoint *endpoint);
PUBLIC void httpAddHost(Http *http, struct HttpHost *host);
PUBLIC void httpRemoveHost(Http *http, struct HttpHost *host);
PUBLIC void httpDefineRouteBuiltins();

/*********************************** HttpStats ********************************/
/** 
    HttpStats
    @defgroup HttpStats HttpStats
    @stability Internal
 */
typedef struct HttpStats {
    uint64  mem;                        /**< Current application memory */
    uint64  memRedline;                 /**< Memory redline limit */
    uint64  memMax;                     /**< Memory maximum permitted */

    uint64  heap;                       /**< Current application heap memory */
    uint64  heapUsed;                   /**< Current heap memory in use */
    uint64  heapFree;                   /**< Current heap memory available */

    int     workersBusy;                /**< Current busy worker threads */
    int     workersIdle;                /**< Current idle worker threads */
    int     workersYielded;             /**< Number of busy workers that are yielded for GC */
    int     workersMax;                 /**< Maximum number of workers in the thread pool */

    int     activeClients;              /**< Current active client IPs */
    int     activeConnections;          /**< Current active connections */
    int     activeProcesses;            /**< Current active processes */
    int     activeRequests;             /**< Current active requests */
    int     activeSessions;             /**< Current active sessions */
    int     activeVMs;                  /**< Current ejs VMs */

    uint64  totalSweeps;                /**< Total GC sweeps */
    uint64  totalRequests;              /**< Total requests served */
    uint64  totalConnections;           /**< Total connections accepted */

    int     pendingRequests;            /**< Pending requests waiting to be served */
    int     regions;                    /**< Current memory region count */
    int     cpus;
} HttpStats;

/** 
    Get an Http performance report
    @param flags reserved
    @return String containing the report
    @ingroup HttpStats
    @stability Internal
 */
PUBLIC char *httpStatsReport(int flags);

/** 
    Get the Http performance statistics
    @param sp Reference to a HttpStats structure
    @ingroup HttpStats
    @stability Internal
 */
PUBLIC void httpGetStats(HttpStats *sp);

/************************************* Limits *********************************/
/** 
    Http limits
    @defgroup HttpLimits HttpLimits
    @see HttpLimits httpInitLimits httpCreateLimits httpEaseLimits
    @stability Internal
 */
typedef struct HttpLimits {
    ssize    bufferSize;                /**< Maximum buffering by any pipeline stage */
    ssize    chunkSize;                 /**< Maximum chunk size for transfer encoding */
    ssize    headerSize;                /**< Maximum size of the total header */
    ssize    uriSize;                   /**< Maximum size of a uri */
    ssize    cacheItemSize;             /**< Maximum size of a cachable item */

    MprOff   receiveFormSize;           /**< Maximum size of form data */
    MprOff   receiveBodySize;           /**< Maximum size of receive body data */
    MprOff   transmissionBodySize;      /**< Maximum size of transmission body content */
    MprOff   uploadSize;                /**< Maximum size of an uploaded file */

    int      clientMax;                 /**< Maximum number of simultaneous clients endpoints */
    int      headerMax;                 /**< Maximum number of header lines */
    int      keepAliveMax;              /**< Maximum number of Keep-Alive requests to perform per socket */
    int      requestMax;                /**< Maximum number of simultaneous concurrent requests */
    int      requestsPerClientMax;      /**< Maximum number of requests per client IP */
    int      processMax;                /**< Maximum number of processes (CGI) */
    int      sessionMax;                /**< Maximum number of sessions */

    MprTicks inactivityTimeout;         /**< Timeout for keep-alive and idle requests (msec) */
    MprTicks requestParseTimeout;       /**< Time a request can take to parse the request headers (msec) */
    MprTicks requestTimeout;            /**< Time a request can take (msec) */
    MprTicks sessionTimeout;            /**< Time a session can persist (msec) */
    MprTicks webSocketsPing;            /**< Time between pings */

    int      webSocketsMax;             /**< Maximum number of WebSockets */
    ssize    webSocketsMessageSize;     /**< Maximum total size of a WebSocket message including all frames */
    ssize    webSocketsFrameSize;       /**< Maximum size of a WebSocket frame on the wire */
    ssize    webSocketsPacketSize;      /**< Maximum size of a WebSocket packet exchanged with the user */
} HttpLimits;

/**
    Initialize a limits object with default values
    @param limits Limits object to modify
    @param serverSide Set to "true" for server side limits. Set to "false" for client side default limits
    @ingroup HttpLimits
    @stability Evolving
 */
PUBLIC void httpInitLimits(HttpLimits *limits, bool serverSide);

/**
    Create a new limits object
    @description Create and initialize a new limits object with default values
    @param serverSide Set to "true" for server side limits. Set to "false" for client side default limits
    @return The allocated limits object
    @ingroup HttpLimits
    @stability Evolving
 */
PUBLIC HttpLimits *httpCreateLimits(int serverSide);

/**
    Ease the limits
    @description This increases the receive body size, transmission body size and upload size to the maximum 
        sizes supported by the system.
    @param limits Limits object. This can be either HttpHost.limits HttpConn.limits or HttpEndpoint.limits
    @ingroup HttpLimits
    @stability Evolving
 */
PUBLIC void httpEaseLimits(HttpLimits *limits);

/************************************* URI Services ***************************/
/** 
    URI management
    @description The HTTP provides routines for formatting and parsing URIs. Routines are also provided
        to escape dangerous characters for URIs as well as HTML content and shell commands.
    @see HttpConn httpCloneUri httpCompleteUri httpCreateUri httpCreateUriFromParts httpFormatUri httpGetRelativeUri 
        httpJoinUri httpJoinUriPath httpLookupMimeType httpMakeUriLocal httpNormalizeUriPath httpResolveUri 
        httpUriToString 
    @defgroup HttpUri HttpUri
    @stability Internal
 */
typedef struct HttpUri {
    char        *scheme;                /**< URI scheme (http|https|...) */
    char        *host;                  /**< Host name */
    char        *path;                  /**< Uri path (without scheme, host, query or fragements) */
    char        *ext;                   /**< Document extension */
    char        *reference;             /**< Reference fragment within the specified resource */
    char        *query;                 /**< Query string */
    int         port;                   /**< Port number */
    int         secure;                 /**< Using https */
    int         webSockets;             /**< Using web sockets */
    char        *uri;                   /**< Original URI (not decoded) */
} HttpUri;

#define HTTP_COMPLETE_URI       0x1     /**< Complete all missing URI fields. Set from "http://localhost/" */
#define HTTP_COMPLETE_URI_PATH  0x2     /**< Complete missing URI path. Set to "/" */

/**
    Clone a URI
    @description This call copies the base URI and optionally completes missing fields in the URI
    @param base Base URI to copy
    @param flags Set to HTTP_COMPLETE_URI to add missing components. ie. Add scheme, host and port if not supplied. 
    @return A new URI object
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC HttpUri *httpCloneUri(HttpUri *base, int flags);

/**
    Complete the given URI
    @description Complete the URI supplying missing URI components from the other URI. This modifies the supplied URI and
        does not allocate or create a new URI.
    @param uri URI to complete
    @param other Other URI to supply the missing components
    @return The supplied URI.
    @ingroup HttpUri
    @stability Stable
  */
PUBLIC HttpUri *httpCompleteUri(HttpUri *uri, HttpUri *other);

/** 
    Create and initialize a URI.
    @description Parse a uri and return a tokenized HttpUri structure.
    @param uri Uri string to parse
    @param flags Set to HTTP_COMPLETE_URI to add missing components. ie. Add scheme, host and port if not supplied. 
    @return A newly allocated HttpUri structure. 
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC HttpUri *httpCreateUri(cchar *uri, int flags);

/**
    Create a URI from parts
    @description This call constructs a URI from the given parts. Various URI parts can be omitted by setting to null.
        The URI path is the only mandatory parameter.
    @param scheme The URI scheme. This is typically "http" or "https".
    @param host The URI host name portion. This can be a textual host and domain name or it can be an IP address.
    @param port The URI port number. Set to zero to accept the default value for the selected scheme.
    @param path The URI path to the requested document.
    @param reference URI reference with an HTML document. This is the URI component after the "#" in the URI path.
    @param query URI query component. This is the URI component after the "?" in the URI.
    @param flags Set to HTTP_COMPLETE_URI to add missing components. ie. Add scheme, host and port if not supplied. 
    @return A new URI 
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC HttpUri *httpCreateUriFromParts(cchar *scheme, cchar *host, int port, cchar *path, cchar *reference, 
        cchar *query, int flags);

/** 
    Format a URI
    @description Format a URI string using the input components.
    @param scheme Protocol string for the uri. Example: "http"
    @param host Host or IP address
    @param port TCP/IP port number
    @param path URL path
    @param ref URL reference fragment
    @param query Additiona query parameters.
    @param flags Set to HTTP_COMPLETE_URI to add missing components. ie. Add scheme, host and port if not supplied. 
    @return A newly allocated uri string
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC char *httpFormatUri(cchar *scheme, cchar *host, int port, cchar *path, cchar *ref, cchar *query, int flags);

/**
    Join URIs
    @param base Base URI to being with
    @param argc Count of URIs in others
    @param others Array of URIs to join to the base
    @return The resulting, joined URI
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC HttpUri *httpJoinUri(HttpUri *base, int argc, HttpUri **others);

/**
    Join a URI path
    @param result URI that will be modified with a joined path
    @param base URI supplying the base path 
    @param other Other URI whose path is joined to the base
    @return The result URI
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC HttpUri *httpJoinUriPath(HttpUri *result, HttpUri *base, HttpUri *other);

/** 
    Get the mime type for an extension.
    This call will return the mime type from a limited internal set of mime types for the given path or extension.
    @param ext Path or extension to examine
    @returns Mime type. This is a static string.
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC cchar *httpLookupMimeType(cchar *ext);

/**
    Normalize a URI
    @description Validate and canonicalize a URI. This invokes httpNormalizeUriPath to normalize the URI path.
    @param uri URI object to normalize
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC void httpNormalizeUri(HttpUri *uri);

/** 
    Normalize a URI
    @description Validate and canonicalize a URI path. This removes redundant "./" sequences and simplifies "../dir" 
        references. 
    @param uri Uri path string to normalize. This is the URI path portion without scheme, host and port components.
    @return A new validated uri string. 
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC char *httpNormalizeUriPath(cchar *uri);

/**
    Get a relative URI from the base to the target
    @description If the target is null, an absolute URI, or if a relative URI from the base cannot be constructed, then
        the target will be returned. If clone is true, then a clone of the target will be returned.
    @param base The base URI considered to be the current URI. Think of this as the current directory.
    @param target The destination URI for which a relative URI will be crafted to reach.
    @param clone If true, the target URI will be cloned if the target is an absolute URI or if a relative URI 
        cannot be constructed.
    @ingroup HttpUri
    @stability Stable
  */
PUBLIC HttpUri *httpGetRelativeUri(HttpUri *base, HttpUri *target, int clone);

/**
    Make a URI local
    @description This routine removes the scheme, host and port portions of a URI
    @param uri URI to modify
    @return The given URI. 
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC HttpUri *httpMakeUriLocal(HttpUri *uri);

/**
    Resolve URIs relative to a base
    @param base Base URI to begin with
    @param argc Count of URIs in the others array
    @param others Array of URIs that are sucessively resolved relative to the base
    @param local If true, the base scheme, host and port are ignored
    @ingroup HttpUri
    @stability Stable
  */
PUBLIC HttpUri *httpResolveUri(HttpUri *base, int argc, HttpUri **others, bool local);

/** 
    Convert a Uri to a string.
    @description Convert the given Uri to a string, optionally completing missing parts such as the host, port and path.
    @param uri A Uri object created via httpCreateUri 
    @param flags Set to HTTP_COMPLETE_URI to add missing components. ie. Add scheme, host and port if not supplied. 
    @return A newly allocated uri string. 
    @ingroup HttpUri
    @stability Stable
 */
PUBLIC char *httpUriToString(HttpUri *uri, int flags);

/************************************* Range **********************************/
/** 
    Content range structure
    @pre
        Range:  0,  49  First 50 bytes
        Range: -1, -50  Last 50 bytes
        Range:  1,  -1  Skip first byte then select content to the end
    @defgroup HttpRange HttpRange
    @see HttpRange
    @stability Internal
 */
typedef struct HttpRange {
    MprOff              start;                  /**< Start of range */
    MprOff              end;                    /**< End byte of range + 1 */
    MprOff              len;                    /**< Redundant range length */
    struct HttpRange    *next;                  /**< Next range */
} HttpRange;

/************************************* Packet *********************************/
/*  
    Packet flags
 */
#define HTTP_PACKET_HEADER    0x1               /**< Packet contains HTTP headers */
#define HTTP_PACKET_RANGE     0x2               /**< Packet is a range boundary packet */
#define HTTP_PACKET_DATA      0x4               /**< Packet contains actual content data */
#define HTTP_PACKET_END       0x8               /**< End of stream packet */
#define HTTP_PACKET_SOLO      0x10              /**< Don't join this packet */

/**
    Callback procedure to fill a packet with data
    @param q Queue owning the packet
    @param packet The packet to fill
    @param off Offset in the packet to fill with data
    @param size Size of packet from the offset to fill.
    @return The number of bytes copied into the packet.
    @ingroup HttpPacket
    @stability Stable
 */
typedef ssize (*HttpFillProc)(struct HttpQueue *q, struct HttpPacket *packet, MprOff pos, ssize size);

/** 
    Packet object. 
    @description The request/response pipeline sends data and control information in HttpPacket objects. The output
        stream typically consists of a HEADER packet followed by zero or more data packets and terminated by an END
        packet. If the request has input data, the input stream consists of one or more data packets followed by
        an END packet.
        \n\n
        Packets contain data and optional prefix or suffix headers. Packets can be split, joined, filled, or emptied. 
        The pipeline stages will fill or transform packet data as required.
    @defgroup HttpPacket HttpPacket
    @see HttpFillProc HttpPacket HttpQueue httpAdjustPacketEnd httpAdjustPacketStart httpClonePacket 
        httpCreateDataPacket httpCreateEndPacket httpCreateEntityPacket httpCreateHeaderPacket httpCreatePacket 
        httpGetPacket httpGetPacketLength httpIsLastPacket httpJoinPacket 
        httpPutBackPacket httpPutForService httpPutPacket httpPutPacketToNext httpSplitPacket 
    @stability Internal
 */
typedef struct HttpPacket {
    MprBuf          *prefix;                /**< Prefix message to be emitted before the content */
    MprBuf          *content;               /**< Chunk content */
    MprOff          esize;                  /**< Data size in entity (file) */
    MprOff          epos;                   /**< Data position in entity (file) */
    HttpFillProc    fill;                   /**< Callback to fill packet with data */
    uint            flags: 7;               /**< Packet flags */
    uint            last: 1;                /**< Last packet in a message */
    uint            type: 24;               /**< Packet type extension */
    struct HttpPacket *next;                /**< Next packet in chain */
} HttpPacket;

/**
    Adjust the packet starting position.
    @description This adjusts the packet content by the given size. The packet position is incremented by start and the
    packet length (size) is decremented. If the packet describes entity data, the given size amount to the Packet.epos and 
    decrements the Packet.esize fields. If the packet has actual data buffered in Packet.content, the content buffer
    start is incremeneted by the size amount.
    @param packet Packet to modify
    @param size Size to add to the packet current position.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC void httpAdjustPacketStart(HttpPacket *packet, MprOff size);

/**
    Adjust the packet end position.
    @description This adjusts the packet content by the given size. The packet length (size) is decremented by the requested 
    amount. If the packet describes entity data, the Packet.esize field is reduced by the requested size amount. If the 
    packet has actual data buffered in Packet.content, the content buffer end position is reduced by
    by the size amount.
    @param packet Packet to modify
    @param size Size to adjust packet end position.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC void httpAdjustPacketEnd(HttpPacket *packet, MprOff size);

/**
    Clone a packet
    @param orig Original packet to clone
    @return A new packet equivalent to the original
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC HttpPacket *httpClonePacket(HttpPacket *orig);

/** 
    Create a data packet
    @description Create a packet and set the HTTP_PACKET_DATA flag
        Data packets convey data through the response pipeline.
    @param size Size of the package data storage.
    @return HttpPacket object.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC HttpPacket *httpCreateDataPacket(ssize size);

/** 
    Create an end-of-stream packet
    @description Create an end-of-stream packet and set the HTTP_PACKET_END flag. The end pack signifies the 
        end of data. It is used on both incoming and outgoing streams through the request/response pipeline.
    @return HttpPacket object.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC HttpPacket *httpCreateEndPacket();

/** 
    Create an entity data packet
    @description Create an entity packet and set the HTTP_PACKET_DATA flag.
        Entity packets describe the resource (entity) to send to the client and provide a #HttpFillProc procedure
        used to fill packets with data from the entity.
    @param pos Position within the entity for packet data 
    @param size Size of the entity data
    @param fill HttpFillProc callback to supply the entity data.
    @return HttpPacket object.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC HttpPacket *httpCreateEntityPacket(MprOff pos, MprOff size, HttpFillProc fill);

/** 
    Create a response header packet
    @description Create a response header packet and set the HTTP_PACKET_HEADER flag. 
        A header packet is used by the pipeline to hold the response headers.
    @return HttpPacket object.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC HttpPacket *httpCreateHeaderPacket();

/** 
    Create a data packet
    @description Create a packet of the required size.
    @param size Size of the package data storage.
    @return HttpPacket object.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC HttpPacket *httpCreatePacket(ssize size);

/** 
    Get the next packet from a queue
    @description Get the next packet. This will remove the packet from the queue and adjust the queue counts
        accordingly. If the queue is full and upstream queues are blocked, they will be enabled.
    @param q Queue reference
    @return The packet removed from the queue.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC HttpPacket *httpGetPacket(struct HttpQueue *q);

#if DOXYGEN
/** 
    Get the length of the packet data contents.
    @description Get the content length of a packet. This does not include the prefix or virtual data length -- just
    the pure buffered data contents. 
    @param packet Packet to examine.
    @return Count of bytes contained by the packet.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC ssize httpGetPacketLength(HttpPacket *packet);
#else
#define httpGetPacketLength(p) ((p && p->content) ? mprGetBufLength(p->content) : 0)
#endif

/**
    Test if the packet is the last in a logical message.
    @description Useful for WebSockets to test if the packet is the last frame in a message
    @param packet Packet to examine
    @return True if the packet is the last in a message.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC bool httpIsLastPacket(HttpPacket *packet);

/** 
    Join two packets
    @description Join the contents of one packet to another by copying the data from the \a other packet into 
        the first packet. 
    @param packet Destination packet
    @param other Other packet to copy data from.
    @return "Zero" if successful, otherwise a negative Mpr error code
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC int httpJoinPacket(HttpPacket *packet, HttpPacket *other);

/** 
    Split a data packet
    @description Split a data packet at the specified offset. Packets may need to be split so that downstream
        stages can digest their contents. If a packet is too large for the queue maximum size, it should be split.
        When the packet is split, a new packet is created containing the data after the offset. Any suffix headers
        are moved to the new packet.
    @param packet Packet to split
    @param offset Route in the original packet at which to split
    @return New HttpPacket object containing the data after the offset. No need to free, unless you have a very long
        running request. Otherwise the packet memory will be released automatically when the request completes.
    @ingroup HttpPacket
    @stability Stable
 */
PUBLIC HttpPacket *httpSplitPacket(HttpPacket *packet, ssize offset);

/*
    Internal
 */
#define httpGetPacketEntityLength(p) (p->content ? mprGetBufLength(p->content) : packet->esize)

/************************************* Queue *********************************/
/*  
    Queue directions
 */
#define HTTP_QUEUE_TX             0           /**< Send (transmit to client) queue */
#define HTTP_QUEUE_RX             1           /**< Receive (read from client) queue */
#define HTTP_MAX_QUEUE            2           /**< Number of queue types */

/* 
   Queue flags
 */
#define HTTP_QUEUE_OPEN           0x1         /**< Queue's open routine has been called */
#define HTTP_QUEUE_SUSPENDED      0x2         /**< Queue's service routine is suspended due to flow control */
#define HTTP_QUEUE_ALL            0x10        /**< Queue has all the data there is and will be */
#define HTTP_QUEUE_SERVICED       0x20        /**< Queue has been serviced at least once */
#define HTTP_QUEUE_EOF            0x40        /**< Queue at end of data */
#define HTTP_QUEUE_STARTED        0x80        /**< Handler stage start routine called */
#define HTTP_QUEUE_READY          0x100       /**< Handler stage ready routine called */
#define HTTP_QUEUE_RESERVICE      0x200       /**< Queue requires reservicing */

/*  
    Queue callback prototypes
 */
typedef void (*HttpQueueOpen)(struct HttpQueue *q);
typedef void (*HttpQueueClose)(struct HttpQueue *q);
typedef void (*HttpQueueStart)(struct HttpQueue *q);
typedef void (*HttpQueueData)(struct HttpQueue *q, HttpPacket *packet);
typedef void (*HttpQueueService)(struct HttpQueue *q);

/** 
    Queue object
    @description The request pipeline consists of a full-duplex pipeline of stages. Each stage has two queues,
        one for outgoing data and one for incoming. A HttpQueue object manages the data flow for a request stage
        and has the ability to queue and process data, manage flow control, and schedule packets for service.
        \n\n
        Queue's provide open, close, put, and service methods. These methods manage and respond to incoming packets.
        A queue can respond immediately to an incoming packet by processing or dispatching a packet in its put() method.
        Alternatively, the queue can defer processing by queueing the packet on it's service queue and then waiting for
        it's service() method to be invoked. 
        \n\n
        If a queue does not define a put() method, the default put() method will 
        be used which queues data onto the service queue. The default incoming put() method joins incoming packets
        into a single packet on the service queue.
        \n\n
        Data flows downstream from one queue to the next queue linked via the nextQ field.
    @defgroup HttpQueue HttpQueue
    @see HttpConn HttpPacket HttpQueue httpDisableQueue httpDiscardQueueData httpEnableQueue httpFlushQueue httpGetQueueRoom
        httpIsEof httpIsPacketTooBig httpIsQueueEmpty httpIsQueueSuspended httpJoinPacketForService httpJoinPackets
        httpPutBackPacket httpPutForService httpPutPacket httpPutPacketToNext httpRemoveQueue httpResizePacket
        httpResumeQueue httpScheduleQueue httpServiceQueue httpSetQueueLimits httpSuspendQueue
        httpWillNextQueueAcceptPacket httpWillNextQueueAcceptSize httpWrite httpWriteBlock httpWriteBody httpWriteString 
    @stability Internal
 */
typedef struct HttpQueue {
    /* Ordered for debugging */
    cchar               *name;                  /**< Queue name for debugging */
    ssize               count;                  /**< Bytes in queue (Does not include virt packet data) */
    int                 flags;                  /**< Queue flags */
    struct HttpQueue    *nextQ;                 /**< Downstream queue for next stage */
    HttpPacket          *first;                 /**< First packet in queue (singly linked) */
    struct HttpConn     *conn;                  /**< Connection owning this queue */
    ssize               max;                    /**< Advisory maxiumum queue size */
    ssize               low;                    /**< Low water mark for flow control */
    ssize               packetSize;             /**< Maximum acceptable packet size */
    HttpPacket          *last;                  /**< Last packet in queue (tail pointer) */
    struct HttpQueue    *prevQ;                 /**< Upstream queue for prior stage */
    struct HttpStage    *stage;                 /**< Stage owning this queue */
    HttpQueueOpen       open;                   /**< Open the queue */
    HttpQueueClose      close;                  /**< Close the queue */
    HttpQueueStart      start;                  /**< Start the queue */
    HttpQueueData       put;                    /**< Callback to receive a packet */
    HttpQueueService    service;                /**< Service the queue */
    struct HttpQueue    *scheduleNext;          /**< Next linkage when queue is on the service queue */
    struct HttpQueue    *schedulePrev;          /**< Previous linkage when queue is on the service queue */
    struct HttpQueue    *pair;                  /**< Queue for the same stage in the opposite direction */
    int                 servicing;              /**< Currently being serviced */
    int                 direction;              /**< Flow direction */
    void                *queueData;             /**< Stage instance data - must be a managed reference */
    void                *staticData;            /**< Stage instance data - must be an unmanaged reference */

    /*  
        Connector instance data
     */
    MprIOVec            iovec[BIT_MAX_IOVEC];
    int                 ioIndex;                /**< Next index into iovec */
    int                 ioFile;                 /**< Sending a file */
    MprOff              ioCount;                /**< Count of bytes in iovec including file I/O */
    MprOff              ioPos;                  /**< Position in file for sendfile */
} HttpQueue;


/** 
    Disable a queue
    @description Disable a queue so that it will not be scheduled for service. The queue will remain disabled until
        httpEnableQueue is called.
    @param q Queue reference
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpDisableQueue(HttpQueue *q);

/** 
    Discard all data from the queue
    @description Discard data from the queue. If removePackets (not yet implemented) is "true", then remove the packets.
        Oherwise, just discard the data and preserve the packets.
    @param q Queue reference
    @param removePackets If "true", the data packets will be removed from the queue.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpDiscardQueueData(HttpQueue *q, bool removePackets);

/** 
    Enable a queue after it has been disabled.
    @description Enable a queue for service and schedule it to run. This will cause the service routine
        to run as soon as possible.
    @param q Queue reference
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpEnableQueue(HttpQueue *q);

/**
    Flush queue data
    @description This initiates writing buffered queue data (flushes) by scheduling the queue and servicing all
    scheduled queues.  If blocking is requested, the call will block until the queue count falls below the queue
    maximum.
    WARNING: Be very careful when using blocking == true. Should only be used by end applications and not by middleware.
    @param q Queue to flush
    @param block If set to "true", this call will block until the data has drained below the queue maximum.
    @return "True" if there is room for more data in the queue after flushing.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC bool httpFlushQueue(HttpQueue *q, bool block);

/** 
    Get the room in the queue
    @description Get the amount of data the queue can accept before being full.
    @param q Queue reference
    @return A count of bytes that can be written to the queue
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC ssize httpGetQueueRoom(HttpQueue *q);

/**
    Test if the connection has received all incoming content
    @description This tests if the connection is at an "End of File condition.
    @param conn HttpConn object created via #httpCreateConn
    @return "True" if all Receive content has been received 
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC bool httpIsEof(struct HttpConn *conn);

/** 
    Test if a packet is too big 
    @description Test if a packet is too big to fit downstream. If the packet content exceeds the downstream queue's 
        maximum or exceeds the downstream queue's requested packet size -- then this routine will return "true".
    @param q Queue reference
    @param packet Packet to test
    @return "True" if the packet is too big for the downstream queue
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC bool httpIsPacketTooBig(struct HttpQueue *q, HttpPacket *packet);

/** 
    Determine if the queue is empty
    @description Determine if the queue has no packets queued. This does not test if the queue has no data content.
    @param q Queue reference
    @return "True" if there are no packets queued.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC bool httpIsQueueEmpty(HttpQueue *q);

/** 
    Test if a queue is suspended.
    @param q Queue reference
    @return true if the queue is suspended.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC bool httpIsQueueSuspended(HttpQueue *q);

/**
    Join packets together
    @description This call joins data packets on the given queue into a single packet. The given size specifies the
    maximum size of data to be joined. The maximum size may also limited by the downstream queue maximum packet size.
    @param q Queue to examine
    @param size The maximum-sized packet that will be created by joining queue packets is the minimum of the given size
        and the downstream queues maximum packet size. Note: this routine will not split packets and so the 
        maximum is advisory only.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpJoinPackets(HttpQueue *q, ssize size);

/** 
    Join a packet onto the service queue
    @description Add a packet to the service queue. If the queue already has data, then this packet
        will be joined (aggregated) into the existing packet. If serviceQ is true, the queue will be scheduled
        for service.
    @param q Queue reference
    @param packet Packet to join to the queue
    @param serviceQ If true, schedule the queue for service
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpJoinPacketForService(struct HttpQueue *q, HttpPacket *packet, bool serviceQ);

/** 
    Put a packet back onto a queue
    @description Put the packet back onto the front of the queue. The queue's put() method is not called.
        This is typically used by the queue's service routine when a packet cannot complete processing.
    @param q Queue reference
    @param packet Packet to put back
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpPutBackPacket(struct HttpQueue *q, HttpPacket *packet);

/*
    Convenience flags for httpPutForService in the serviceQ argument
 */
#define HTTP_DELAY_SERVICE      0           /**< Delay servicing the queue */
#define HTTP_SCHEDULE_QUEUE     1           /**< Schedule the queue for service */

/** 
    Put a packet into the service queue for deferred processing.
    @description Add a packet to the service queue. If serviceQ is true, the queue will be scheduled for service.
    @param q Queue reference
    @param packet Packet to join to the queue
    @param serviceQ If true, schedule the queue for service
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpPutForService(struct HttpQueue *q, HttpPacket *packet, bool serviceQ);

/** 
    Put a packet to the queue.
    @description The packet is passed to the queue by invoking its put() callback. 
        Note the receiving queue may immediately process the packet or it may choose to defer processing by putting to
        its service queue.  @param q Queue reference
    @param packet Packet to put
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpPutPacket(struct HttpQueue *q, HttpPacket *packet);

/** 
    Put a packet to the next queue downstream.
    @description Put a packet onto the next downstream queue by calling the downstream queue's put() method. 
        Note the receiving queue may immediately process the packet or it may choose to defer processing by putting to
        its service queue.  @param q Queue reference
    @param q Queue reference. The packet will not be queued on this queue, but rather on the queue downstream.
    @param packet Packet to put
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpPutPacketToNext(struct HttpQueue *q, HttpPacket *packet);

/** 
    Remove a queue
    @description Remove a queue from the request/response pipeline. This will remove a queue so that it does
        not participate in the pipeline, effectively removing the processing stage from the pipeline. This is
        useful to remove unwanted filters and to speed up pipeline processing
    @param q Queue reference
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpRemoveQueue(HttpQueue *q);

/** 
    Resize a packet
    @description Resize a packet, if required, so that it fits in the downstream queue. This may split the packet
        if it is too big to fit in the downstream queue. If it is split, the tail portion is put back on the queue.
    @param q Queue reference
    @param packet Packet to put
    @param size If size is > 0, then also ensure the packet is not larger than this size.
    @return "Zero" if successful, otherwise a negative Mpr error code
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC int httpResizePacket(struct HttpQueue *q, HttpPacket *packet, ssize size);

/** 
    Resume a queue
    @description Resume a queue for service and schedule it to run. This will cause the service routine
        to run as soon as possible. This is normally called automatically called by the pipeline when downstream
        congestion has cleared.
    @param q Queue reference
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpResumeQueue(HttpQueue *q);

/** 
    Schedule a queue
    @description Schedule a queue by adding it to the schedule queue. Queues are serviced FIFO.
    @param q Queue reference
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpScheduleQueue(HttpQueue *q);

/** 
    Service a queue
    @description Service a queue by invoking its service() routine. 
    @param q Queue reference
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpServiceQueue(HttpQueue *q);

/**
    Set a queue's flow control low and high water marks
    @param q Queue reference
    @param low The low water mark. Typically 5% of the max.
    @param max The high water mark.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpSetQueueLimits(HttpQueue *q, ssize low, ssize max);

/** 
    Suspend a queue. 
    @description Suspended a queue so that it will not be scheduled for service. The pipeline will 
    will automatically call httpResumeQueue when the downstream queues are less congested.
    @param q Queue reference
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC void httpSuspendQueue(HttpQueue *q);

#if BIT_DEBUG
/**
    Verify a queue 
    @param q Queue reference
    @return "True" if the queue verifies
    @internal
 */
PUBLIC bool httpVerifyQueue(HttpQueue *q);
#define VERIFY_QUEUE(q) httpVerifyQueue(q)
#else
#define VERIFY_QUEUE(q)
#endif

/** 
    Determine if the downstream queue will accept this packet.
    @description Test if the downstream queue will accept a packet. The packet will be resized, if required, in an
        attempt to get the downstream queue to accept it. If the downstream queue is full, disable this queue
        and mark the downstream queue as full, and service it immediately to try to relieve the congestion.
    @param q Queue reference
    @param packet Packet to put
    @return "True" if the downstream queue will accept the packet. Use #httpPutPacketToNext to send the packet downstream
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC bool httpWillNextQueueAcceptPacket(HttpQueue *q, HttpPacket *packet);

/** 
    Determine if the given queue will accept this packet.
    @description Test if the queue will accept a packet. The packet will be resized, if split is true, in an
        attempt to get the downstream queue to accept it. 
    @param q Queue reference
    @param packet Packet to put
    @param split Set to true to split the packet if required to fit into the queue.
    @return "True" if the queue will accept the packet. 
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC bool httpWillQueueAcceptPacket(HttpQueue *q, HttpPacket *packet, bool split);

/** 
    Determine if the downstream queue will accept a certain amount of data.
    @description Test if the downstream queue will accept data of a given size.
    @param q Queue reference
    @param size Size of data to test for
    @return "True" if the downstream queue will accept the given sized data.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC bool httpWillNextQueueAcceptSize(HttpQueue *q, ssize size);

/** 
    Write a formatted string
    @description Write a formatted string of data into packets onto the end of the queue. Data packets will be created
        as required to store the write data. This call always accepts all the data and will buffer as required. 
        This call may block waiting for the downstream queue to drain if it is or becomes full.
        Data written after #httpFinalizeOutput or #httpError is called will be ignored.
    @param q Queue reference
    @param fmt Printf style formatted string
    @param ... Arguments for fmt
    @return A count of the bytes actually written
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC ssize httpWrite(HttpQueue *q, cchar *fmt, ...);


#define HTTP_BUFFER     0x1    /**< Flag for httpSendBlock and httpWriteBlock to always absorb the data without blocking */
#define HTTP_BLOCK      0x2    /**< Flag for httpSendBlock and httpWriteBlock to indicate blocking operation */
#define HTTP_NON_BLOCK  0x4    /**< Flag for httpSendBlock and httpWriteBlock to indicate non-blocking operation */

/** 
    Write a block of data to the queue
    @description Write a block of data onto the end of the queue. This will queue the data an may initiaite writing
        to the connection if the queue is full. Data will be appended to last packet in the queue
        if there is room. Otherwise, data packets will be created as required to store the write data. This call operates
        in buffering mode by default unless either the HTTP_BLOCK OR HTTP_NON_BLOCK flag is specified. When blocking, the
        call will either accept and write all the data or it will fail, it will never return "short" with a partial write.
        In blocking mode (HTTP_BLOCK), it block for up to the inactivity timeout specified in the
        conn->limits->inactivityTimeout value.  In non-blocking mode (HTTP_NON_BLOCK), the call may return having written
        fewer bytes than requested. In buffering mode (HTTP_BUFFER), the data is always absorbed without blocking 
        and queue size limits are ignored.
        Data written after #httpFinalize, #httpFinalizeOutput or #httpError is called will be ignored.
    @param q Queue reference
    @param buf Buffer containing the write data
    @param size of the data in buf
    @param flags Set to HTTP_BLOCK for blocking operation or HTTP_NON_BLOCK for non-blocking. Set to HTTP_BUFFER to
        buffer the data if required and never block. Set to zero will default to HTTP_BUFFER.
    @return The size value if successful or a negative MPR error code.
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC ssize httpWriteBlock(HttpQueue *q, cchar *buf, ssize size, int flags);

/** 
    Write a string of data to the queue
    @description Write a string of data into packets onto the end of the queue. Data packets will be created
        as required to store the write data. This call may block waiting for the downstream queue to drain if it is 
        or becomes full.
        Data written after #httpFinalizeOutput or #httpError is called will be ignored.
    @param q Queue reference
    @param s String containing the data to write
    @return A count of the bytes actually written
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC ssize httpWriteString(HttpQueue *q, cchar *s);

/** 
    Write a safe string of data to the queue
    @description This will escape any HTML sequences before writing the string into packets onto the end of the queue. 
        Data packets will be created as required to store the write data. This call may block waiting for the 
        downstream queue to drain if it is or becomes full.
        Data written after #httpFinalizeOutput or #httpError is called will be ignored.
    @param q Queue reference
    @param s String containing the data to write
    @return A count of the bytes actually written
    @ingroup HttpQueue
    @stability Stable
 */
PUBLIC ssize httpWriteString(HttpQueue *q, cchar *s);

/* Internal */
PUBLIC HttpQueue *httpFindPreviousQueue(HttpQueue *q);
PUBLIC HttpQueue *httpCreateQueueHead(struct HttpConn *conn, cchar *name);
PUBLIC HttpQueue *httpCreateQueue(struct HttpConn *conn, struct HttpStage *stage, int dir, HttpQueue *prev);
PUBLIC HttpQueue *httpGetNextQueueForService(HttpQueue *q);
PUBLIC void httpInitQueue(struct HttpConn *conn, HttpQueue *q, cchar *name);
PUBLIC void httpInitSchedulerQueue(HttpQueue *q);
PUBLIC void httpAppendQueue(HttpQueue *prev, HttpQueue *q);
PUBLIC void httpMarkQueueHead(HttpQueue *q);
PUBLIC void httpAssignQueue(HttpQueue *q, struct HttpStage *stage, int dir);

/******************************** Pipeline Stages *****************************/
/*
    Stage Flags
 */
#define HTTP_STAGE_CONNECTOR      0x1000            /**< Stage is a connector  */
#define HTTP_STAGE_HANDLER        0x2000            /**< Stage is a handler  */
#define HTTP_STAGE_FILTER         0x4000            /**< Stage is a filter  */
#define HTTP_STAGE_MODULE         0x8000            /**< Stage is a filter  */
#define HTTP_STAGE_AUTO_DIR       0x10000           /**< Want auto directory redirection */
#define HTTP_STAGE_UNLOADED       0x20000           /**< Stage module library has been unloaded */
#define HTTP_STAGE_RX             0x40000           /**< Stage to be used in the Rx direction */
#define HTTP_STAGE_TX             0x80000           /**< Stage to be used in the Tx direction */

typedef int (*HttpParse)(Http *http, cchar *key, char *value, void *state);

/** 
    Pipeline Stages
    @description The request pipeline consists of a full-duplex pipeline of stages. 
        Stages are used to process client HTTP requests in a modular fashion. Each stage either creates, filters or
        consumes data packets. The HttpStage structure describes the stage capabilities and callbacks.
        Each stage has two queues, one for outgoing data and one for incoming data.
        \n\n
        Stages provide callback methods for parsing configuration, matching requests, open/close, run and the
        acceptance and service of incoming and outgoing data.
    @defgroup HttpStage HttpStage 
    @see HttpConn HttpQueue HttpStage httpCloneStage httpCreateConnector httpCreateFilter httpCreateHandler 
        httpCreateStage httpDefaultOutgoingServiceStage httpGetStageData httpHandleOptionsTrace httpLookupStage 
        httpLookupStageData httpSetStageData 
    @stability Internal
 */
typedef struct HttpStage {
    char            *name;                  /**< Stage name */
    char            *path;                  /**< Backing module path (from LoadModule) */
    int             flags;                  /**< Stage flags */
    void            *stageData;             /**< Private stage data */
    MprModule       *module;                /**< Backing module */
    MprHash         *extensions;            /**< Matching extensions for this filter */
     
    /*  These callbacks apply to all stages */

    /** 
        Match a request
        @description This routine is invoked to see if the stage wishes to handle the request. For handlers, 
            the match callback is invoked when selecting the appropriate route for the request. For filters, 
            the callback is invoked subsequently when constructing the request pipeline.
            If a filter declines to handle a request, the filter will be removed from the pipeline for the 
            specified direction. The direction argument should be ignored for handlers.
            Handlers and filters must not actually handle the request in the match callback and must not call httpError.
            Errors can be reported via mprError. Handlers can defer error reporting until their start callback.
        @param conn HttpConn connection object
        @param route Route object
        @param dir Queue direction. Set to HTTP_QUEUE_TX or HTTP_QUEUE_RX. Always set to HTTP_QUEUE_TX for handlers.
        @return HTTP_ROUTE_OK if the request is acceptable. Return HTTP_ROUTE_REROUTE if the request has been rewritten.
            Return HTTP_ROUTE_REJECT it the request is not acceptable.
        @ingroup HttpStage
        @stability Evolving
      */
    int (*match)(struct HttpConn *conn, struct HttpRoute *route, int dir);

    /** 
        Rewrite a request after matching.
        @description This callback will be invoked for handlers after matching and selecting the handler.
        @param conn HttpConn connection object
        @ingroup HttpStage
        @stability Evolving
     */
    int (*rewrite)(struct HttpConn *conn);

    /** 
        Open the stage
        @description Open the stage for this request instance. A handler may service the request in the open routine
            and may call #httpError if required.
        @param q Queue instance object
        @ingroup HttpStage
        @stability Evolving
     */
    void (*open)(HttpQueue *q);

    /** 
        Close the stage
        @description Close the stage and cleanup any request resources.
        @param q Queue instance object
        @ingroup HttpStage
        @stability Evolving
     */
    void (*close)(HttpQueue *q);

    /** 
        Process outgoing data.
        @description Accept a packet as outgoing data. Not used by handlers as handler generate packets internally.
            Filters will use this entry point to accept outgoing packets.
            Filters can choose to immediately process or forward the packet, or they can queue the packet on their queue and
            schedule their outgoingService callback for batch processing of all queued packets. This is a common pattern
            where the outgoing routine is not used and packets are automatically queued and the outgoingService 
            callback is used to process data.
        @param q Queue instance object
        @param packet Packet of data
        @ingroup HttpStage
        @stability Evolving
     */
    void (*outgoing)(HttpQueue *q, HttpPacket *packet);

    /** 
        Service the outgoing data queue
        @description This callback should service packets on the queue and process or forward as appropriate.
        A service routine should check downstream queues by calling #httpWillNextQueueAcceptPacket before forwarding packets
        to ensure they do not overfow downstream queues.
        @param q Queue instance object
        @ingroup HttpStage
        @stability Evolving
     */
    void (*outgoingService)(HttpQueue *q);

    /** 
        Process incoming data.
        @description Accept an incoming packet of data. 
            Filters and handlers recieve packets via their incoming callback. They can choose to immediately process or
            forward the packet, or they can queue the packet on their queue and schedule their incomingService callback
            for batch processing of all queued packets. This is a common pattern where the incoming routine is not 
            used and packets are automatically queued and the incomingService callback is used to process.
        Not used by connectors.
        @param q Queue instance object
        @param packet Packet of data
        @ingroup HttpStage
        @stability Evolving
     */
    void (*incoming)(HttpQueue *q, HttpPacket *packet);

    /** 
        Service the incoming data queue
        @description This callback should service packets on the queue and process or forward as appropriate.
        A service routine should check upstream queues by calling #httpWillNextQueueAcceptPacket before forwarding packets
        to ensure they do not overfow upstream queues.
        @param q Queue instance object
        @ingroup HttpStage
        @stability Evolving
     */
    void (*incomingService)(HttpQueue *q);


    /*  These callbacks apply only to handlers */

    /** 
        Start the handler
        @description The start callback is primarily responsible for starting the request processing. 
        Depending on the request Content Type, the request will be started at different times. 
        Form requests with a Content-Type of "application/x-www-form-urlencoded", will be started after fully receiving all
        input data. Other requests will be started immediately after the request headers have been parsed and before
        receiving input data. This enables such requests to stream large quantities of input data without buffering.
        The handler start callback should test the HTTP method in conn->rx->method and only respond to supported HTTP
        methods. It should call httpError for unsupported methods.
        The start callback will not be called if the request already has an error.
        @param q Queue instance object
        @ingroup HttpStage
        @stability Evolving
     */
    void (*start)(HttpQueue *q);

    /** 
        The request is now fully ready.
        @description This callback will be invoked when all incoming data has been received. 
            The ready callback will not be called if the request already has an error.
            If a handler finishes processing the request, it should call #httpFinalizeOutput in the ready routine.
        @param q Queue instance object
        @ingroup HttpStage
        @stability Evolving
     */
    void (*ready)(HttpQueue *q);

    /** 
        The outgoing pipeline is writable and can accept more response data.
        @description This callback will be invoked after all incoming data has been receeived and whenever the outgoing
        pipeline can absorb more output data (writable). As such, it may be called multiple times and can be effectively
        used for non-blocking generation of a response.
        The writable callback will not be invoked if the request output has been finalized or if an error has occurred.
        @param q Queue instance object
        @ingroup HttpStage
        @stability Evolving
     */
    void (*writable)(HttpQueue *q);

} HttpStage;


/**
    Create a clone of an existing state. This is used when creating filters configured to match certain extensions.
    @param http Http object returned from #httpCreate
    @param stage Stage object to clone
    @return A new stage object
    @ingroup HttpStage
    @stability Stable
*/
PUBLIC HttpStage *httpCloneStage(Http *http, HttpStage *stage);

/** 
    Create a connector stage
    @description Create a new connector. Connectors are the final stage for outgoing data. Their job is to transmit
        outgoing data to the client.
    @param http Http object returned from #httpCreate
    @param name Name of connector stage
    @param module Optional module object for loadable stages
    @return A new stage object
    @ingroup HttpStage
    @stability Stable
 */
PUBLIC HttpStage *httpCreateConnector(Http *http, cchar *name, MprModule *module);

/** 
    Create a filter stage
    @description Create a new filter. Filters transform data generated by handlers and before connectors transmit to
        the client. Filters can apply transformations to incoming, outgoing or bi-directional data.
    @param http Http object
    @param name Name of connector stage
    @param module Optional module object for loadable stages
    @return A new stage object
    @ingroup HttpStage
    @stability Stable
 */
PUBLIC HttpStage *httpCreateFilter(Http *http, cchar *name, MprModule *module);

/** 
    Create a request handler stage
    @description Create a new handler. Handlers generate outgoing data and are the final stage for incoming data. 
        Their job is to process requests and send outgoing data downstream toward the client consumer.
        There is ever only one handler for a request.
    @param http Http object
    @param name Name of connector stage
    @param module Optional module object for loadable stages
    @return A new stage object
    @ingroup HttpStage
    @stability Stable
 */
PUBLIC HttpStage *httpCreateHandler(Http *http, cchar *name, MprModule *module);

/** 
    Create a connector stage
    @description Create a new stage.
    @param http Http object returned from #httpCreate
    @param name Name of connector stage
    @param flags Stage flags
    @param module Optional module object for loadable stages
    @return A new stage object
    @ingroup HttpStage
    @stability Stable
 */
PUBLIC HttpStage *httpCreateStage(Http *http, cchar *name, int flags, MprModule *module);

/** 
    Lookup a stage by name
    @param http Http object
    @param name Name of stage to locate
    @return Stage or NULL if not found
    @ingroup HttpStage
    @stability Stable
*/
PUBLIC struct HttpStage *httpLookupStage(Http *http, cchar *name);

/** 
    Default outgoing data handling
    @description This routine provides default handling of outgoing data for stages. It simply sends all packets
        downstream.
    @param q Queue object
    @ingroup HttpStage
    @stability Stable
 */
PUBLIC void httpDefaultOutgoingServiceStage(HttpQueue *q);

/**
    Get stage data   
    @description Stages can store extra configuration information indexed by key. This is used by handlers, filters,
        connectors and and handlers.
    @param conn HttpConn connection object
    @param key Key index into the stage data
    @return A reference to the stage data. Otherwise return null if the route data for the given key was not found.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC cvoid *httpGetStageData(struct HttpConn *conn, cchar *key);

/**
    Handle a Http Trace or Options method request
    @description Convenience routine to respond to an OPTIONS or TRACE request. 
    @param conn HttpConn object created via #httpCreateConn
    @param methods Comma separated list of supported methods excluding OPTIONS and TRACE which are automatically
        added if the route supports these methods.
    @ingroup HttpStage
    @stability Evolving
 */
PUBLIC void httpHandleOptionsTrace(struct HttpConn *conn, cchar *methods);

/** 
    Lookup stage data
    @description This looks up the stage by name and returns the private stage data.
    @param http Http object
    @param name Name of the stage concerned
    @return Reference to the stage data block.
    @ingroup HttpStage
    @stability Stable
 */
PUBLIC void *httpLookupStageData(Http *http, cchar *name);

/**
    Set stage data   
    @description Stages can store extra configuration information indexed by key. This is used by handlers, filters,
        connectors and and handlers.
    @param conn HttpConn connection object
    @param key Key index into the stage data
    @param data Reference to custom data allocated via mprAlloc.
    @ingroup HttpRoute
    @stability Stable
 */
PUBLIC void httpSetStageData(struct HttpConn *conn, cchar *key, cvoid *data);

/* Internal APIs */
PUBLIC void httpAddStage(Http *http, HttpStage *stage);
PUBLIC ssize httpFilterChunkData(HttpQueue *q, HttpPacket *packet);
PUBLIC int httpOpenActionHandler(Http *http);
PUBLIC int httpOpenChunkFilter(Http *http);
PUBLIC int httpOpenCacheHandler(Http *http);
PUBLIC int httpOpenPassHandler(Http *http);
PUBLIC int httpOpenRangeFilter(Http *http);
PUBLIC int httpOpenNetConnector(Http *http);
PUBLIC int httpOpenSendConnector(Http *http);
PUBLIC int httpOpenUploadFilter(Http *http);
PUBLIC int httpOpenWebSockFilter(Http *http);
PUBLIC void httpSendOpen(HttpQueue *q);
PUBLIC void httpSendOutgoingService(HttpQueue *q);

/********************************** HttpConn *********************************/
/** 
    Notifier events
 */
#define HTTP_EVENT_STATE            1       /**< The request is changing state */
#define HTTP_EVENT_READABLE         2       /**< The request has data available for reading */
#define HTTP_EVENT_WRITABLE         3       /**< The request is now writable (post / put data) */
#define HTTP_EVENT_ERROR            4       /**< The request has an error */
#define HTTP_EVENT_DESTROY          5       /**< The connection is being closed and destroyed */

/*
    Application level events 
 */
#define HTTP_EVENT_APP_OPEN         6       /**< The request is now open */
#define HTTP_EVENT_APP_CLOSE        7       /**< The request is now closed */
#define HTTP_EVENT_MAX              8

/*  
    Connection / Request states
    It is critical that the states be ordered and the values be contiguous. The httpSetState relies on this.
 */
#define HTTP_STATE_BEGIN            1       /**< Ready for a new request */
#define HTTP_STATE_CONNECTED        2       /**< Connection received or made */
#define HTTP_STATE_FIRST            3       /**< First request line has been parsed */
#define HTTP_STATE_PARSED           4       /**< Headers have been parsed, handler can start */
#define HTTP_STATE_CONTENT          5       /**< Reading posted content */
#define HTTP_STATE_READY            6       /**< Handler ready - all body data received  */
#define HTTP_STATE_RUNNING          7       /**< Handler running */
#define HTTP_STATE_FINALIZED        8       /**< Input received, request processed and response transmitted */
#define HTTP_STATE_COMPLETE         9       /**< Request complete */

/*
    Limit validation events
 */
#define HTTP_VALIDATE_OPEN_CONN     1       /**< Open a new connection */
#define HTTP_VALIDATE_CLOSE_CONN    2       /**< Close a connection */
#define HTTP_VALIDATE_OPEN_REQUEST  3       /**< Open a new request */
#define HTTP_VALIDATE_CLOSE_REQUEST 4       /**< Close a request */
#define HTTP_VALIDATE_OPEN_PROCESS  5       /**< Open a new CGI process */
#define HTTP_VALIDATE_CLOSE_PROCESS 6       /**< Close a CGI process */

/*
    Trace directions
 */
#define HTTP_TRACE_RX               0       /**< Trace reception */
#define HTTP_TRACE_TX               1       /**< Trace transmission */
#define HTTP_TRACE_MAX_DIR          2       /**< Trace transmission */

/*
    Trace items
 */
#define HTTP_TRACE_CONN             0       /**< New connections */
#define HTTP_TRACE_FIRST            1       /**< First line of header only */
#define HTTP_TRACE_HEADER           2       /**< Header */
#define HTTP_TRACE_BODY             3       /**< Body content */
#define HTTP_TRACE_LIMITS           4       /**< Request and connection count limits */
#define HTTP_TRACE_TIME             5       /**< Instrument http pipeline */
#define HTTP_TRACE_MAX_ITEM         6

/**
    Trace management structure
    @defgroup HttpTrace HttpTrace
    @stability Internal
  */
typedef struct HttpTrace {
    int             disable;                     /**< If tracing is disabled for this request */
    int             levels[HTTP_TRACE_MAX_ITEM]; /**< Level at which to trace this item */
    MprOff          size;                        /**< Maximum size of content to trace */
    MprHash         *include;                    /**< Extensions to include in trace */
    MprHash         *exclude;                    /**< Extensions to exclude from trace */
} HttpTrace;

PUBLIC void httpManageTrace(HttpTrace *trace, int flags);

/**
    Callback to fill headers 
    @description If defined, the headers callback will run before the standard response headers are generated. This gives an 
    opportunity to pre-populate the response headers.
    @param arg Argument provided to httpSetHeadersCallback when the callback was established.
    @ingroup HttpConn
    @stability Evolving
 */
typedef int (*HttpHeadersCallback)(void *arg);

/**
    Define a headers callback
    @description The headers callback will run before the standard response headers are generated. This gives an 
        opportunity to pre-populate the response headers.
    @param conn HttpConn object created via #httpCreateConn
    @param fn Callback function to invoke
    @param arg Argument to provide when invoking the headers callback
    @ingroup HttpConn
    @stability Evolving
 */
PUBLIC void httpSetHeadersCallback(struct HttpConn *conn, HttpHeadersCallback fn, void *arg);

/**
    I/O callback for connections
    @param conn HttpConn object created via #httpCreateConn
    @param event Event object describing the I/O event
    @ingroup HttpConn
    @stability Evolving
  */
typedef void (*HttpIOCallback)(struct HttpConn *conn, MprEvent *event);

/**
    Define an I/O callback for connections
    @description The I/O callback is invoked when I/O events are detected on the connection. The default I/O callback
        is #httpEvent.
    @param conn HttpConn object created via #httpCreateConn
    @param fn Callback function to invoke
    @ingroup HttpConn
    @stability Evolving
  */
PUBLIC void httpSetIOCallback(struct HttpConn *conn, HttpIOCallback fn);

/** 
    Http Connections
    @description The HttpConn object represents a TCP/IP connection to the client. A connection object is created for
        each socket connection initiated by the client. One HttpConn object may service many Http requests due to 
        HTTP/1.1 keep-alive.
    @defgroup HttpConn HttpConn
    @see HttpConn HttpEnvCallback HttpGetPassword HttpListenCallback HttpNotifier HttpQueue HttpRedirectCallback 
        HttpRx HttpStage HttpTx HtttpListenCallback httpCallEvent httpCloseConn httpFinalizeConnector httpConnTimeout
        httpCreateConn httpCreateRxPipeline httpCreateTxPipeline httpDestroyConn httpDestroyPipeline httpDiscardData
        httpDisconnect httpEnableUpload httpError httpEvent httpGetAsync httpGetChunkSize httpGetConnContext httpGetConnHost
        httpGetError httpGetExt httpGetKeepAliveCount httpGetMoreOutput httpGetWriteQueueCount httpMatchHost httpMemoryError
        httpAfterEvent httpPrepClientConn httpResetCredentials httpRouteRequest httpRunHandlerReady httpServiceQueues
        httpSetAsync httpSetChunkSize httpSetConnContext httpSetConnHost httpSetConnNotifier httpSetCredentials
        httpSetKeepAliveCount httpSetProtocol httpSetRetries httpSetSendConnector httpSetState httpSetTimeout
        httpSetTimestamp httpShouldTrace httpStartPipeline
    @stability Internal
 */
typedef struct HttpConn {
    /*  Ordered for debugability */

    int             state;                  /**< Connection state */
    int             error;                  /**< A request error has occurred */
    int             connError;              /**< A connection error has occurred */
    int             pumping;                /**< Rre-entrancy prevention for httpPumpRequest() */

    struct HttpRx   *rx;                    /**< Rx object */
    struct HttpTx   *tx;                    /**< Tx object */
    HttpQueue       *readq;                 /**< End of the read pipeline */
    HttpQueue       *writeq;                /**< Start of the write pipeline */

    MprSocket       *sock;                  /**< Underlying socket handle */
    HttpLimits      *limits;                /**< Service limits */
    Http            *http;                  /**< Http service object  */
    MprDispatcher   *dispatcher;            /**< Event dispatcher */
    MprDispatcher   *newDispatcher;         /**< New dispatcher if using a worker thread */
    MprDispatcher   *oldDispatcher;         /**< Original dispatcher if using a worker thread */
    HttpNotifier    notifier;               /**< Connection Http state change notification callback */

    struct HttpQueue *serviceq;             /**< List of queues that require service for request pipeline */
    struct HttpQueue *currentq;             /**< Current queue being serviced (just for GC) */
    struct HttpEndpoint *endpoint;          /**< Endpoint object (if set - indicates server-side) */
    struct HttpHost *host;                  /**< Host object (if relevant) */

    HttpPacket      *input;                 /**< Header packet */
    ssize           newData;                /**< Length of new data last read into the input packet */
    HttpQueue       *connectorq;            /**< Connector write queue */
    MprTicks        started;                /**< When the connection started (ticks) */
    MprTicks        lastActivity;           /**< Last activity on the connection */
    MprEvent        *timeoutEvent;          /**< Connection or request timeout event */
    MprEvent        *workerEvent;           /**< Event for running connection via a worker thread */
    void            *context;               /**< Embedding context (EjsRequest) */
    void            *ejs;                   /**< Embedding VM */
    void            *pool;                  /**< Pool of VMs */
    void            *mark;                  /**< Reference for GC marking */
    void            *data;                  /**< Custom data for request - must be a managed reference*/
    void            *staticData;            /**< Custom data for request - must be an - unmanaged reference*/
    void            *grid;                  /**< Current request database grid for MVC apps */
    void            *record;                /**< Current request database record for MVC apps */
    char            *boundary;              /**< File upload boundary */
    char            *errorMsg;              /**< Error message for the last request (if any) */
    char            *ip;                    /**< Remote client IP address */
    char            *protocol;              /**< HTTP protocol */
    char            *protocols;             /**< Supported web socket protocols (clients) */
    int             async;                  /**< Connection is in async mode (non-blocking) */
    int             followRedirects;        /**< Follow redirects for client requests */
    int             keepAliveCount;         /**< Count of remaining Keep-Alive requests for this connection */
    int             http10;                 /**< Using legacy HTTP/1.0 */
    int             port;                   /**< Remote port */
    int             retries;                /**< Client request retries */
    int             secure;                 /**< Using https */
    int             seqno;                  /**< Unique connection sequence number */
    int             upgraded;               /**< Request protocol upgraded */
    int             worker;                 /**< Use worker */

    HttpTrace       trace[2];               /**< Tracing for [rx|tx] */

    /*  
        Authentication
     */
    int             authRequested;          /**< Authorization requested based on user credentials */
    char            *authType;              /**< Type of authentication: set to basic, digest, post or a custom name */
    void            *authData;              /**< Authorization state data */
    char            *username;              /**< Supplied user name */
    char            *password;              /**< Supplied password (may be encrypted depending on auth protocol) */
    int             encoded;                /**< True if the password is MD5(username:realm:password) */
    struct HttpUser *user;                  /**< Authorized User record for access checking */

    HttpTimeoutCallback timeoutCallback;    /**< Request and inactivity timeout callback */
    HttpIOCallback  ioCallback;             /**< I/O event callback */
    HttpHeadersCallback headersCallback;    /**< Callback to fill headers */
    void            *headersCallbackArg;    /**< Arg to fillHeaders */

    uint64          startMark;              /**< High resolution tick time of request */
} HttpConn;


/**
    Call httpEvent with the given event mask
    @param conn HttpConn object created via #httpCreateConn
    @param mask Mask of events. MPR_READABLE | MPR_WRITABLE
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpCallEvent(HttpConn *conn, int mask);

/** 
    Close a connection
    @param conn HttpConn object created via #httpCreateConn
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpCloseConn(HttpConn *conn);

/**
    Finalize connector output sending the response.
    @param conn HttpConn object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
    @internal
 */ 
PUBLIC void httpFinalizeConnector(HttpConn *conn);

/**
    Signal a connection timeout on a connection
    @description This call cancels a connections current request, disconnects the socket and issues an error to the error 
        log. This call is normally invoked by the httpTimer which runs regularly to check for timed out requests.
        This call should not be made on another thread, but should be scheduled to run on the connection's dispatcher to
        avoid thread races.
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
  */
PUBLIC void httpConnTimeout(HttpConn *conn);

/** 
    Create a connection object.
    @description Most interactions with the Http library are via a connection object. It is used for server-side 
        communications when responding to client requests and it is used to initiate outbound client requests.
    @param http Http object created via #httpCreate
    @param endpoint Endpoint object owning the connection.
    @param dispatcher Disptacher to use for I/O events on the connection
    @returns A new connection object
    @ingroup HttpConn
    @stability Internal
*/
PUBLIC HttpConn *httpCreateConn(Http *http, struct HttpEndpoint *endpoint, MprDispatcher *dispatcher);

/**
    Create the receive request pipeline
    @param conn HttpConn object created via #httpCreateConn
    @param route Route object controlling how the pipeline is configured for the request
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpCreateRxPipeline(HttpConn *conn, struct HttpRoute *route);

/**
    Create the transmit request pipeline
    @param conn HttpConn object created via #httpCreateConn
    @param route Route object controlling how the pipeline is configured for the request
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpCreateTxPipeline(HttpConn *conn, struct HttpRoute *route);

/**
    Destroy the connection object
    @param conn HttpConn object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpDestroyConn(HttpConn *conn);

/**
    Destroy the request pipeline. 
    @description This is called at the conclusion of a request.
    @param conn HttpConn object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpDestroyPipeline(HttpConn *conn);

/**
    Discard buffered transmit pipeline data
    @param conn HttpConn object created via #httpCreateConn
    @param dir Queue direction. Either HTTP_QUEUE_TX or HTTP_QUEUE_RX.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpDiscardData(HttpConn *conn, int dir);

/**
    Disconnect the connection's socket
    @description This call will close the socket and signal a connection error. Subsequent use of the connection socket
        will not be possible.  This call should not be made on another thread, but should be scheduled to run on the 
        connection's dispatcher to avoid thread races.
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpConn
    @stability Stable
  */
PUBLIC void httpDisconnect(HttpConn *conn);

/**
    Enable connection events
    @description Connection events are automatically disabled upon receipt of an I/O event on a connection. This 
        permits a connection to process the I/O without fear of interruption by another I/O event. At the completion
        of processing of the I/O request, the connection should be re-enabled via httpEnableConnEvents. This call is
        made for requests in #httpEvent.
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpEnableConnEvents(HttpConn *conn);

/** 
    Enable Multipart-Mime File Upload for this request. This will define a "Content-Type: multipart/form-data..."
    header and will create a mime content boundary for use to delimit the various upload content files and fields.
    @param conn HttpConn connection object
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpEnableUpload(HttpConn *conn);

/** 
    Error handling for the connection.
    @description The httpError call is used to flag the current request as failed. If httpError is called multiple
        times, those calls are ignored and only the first call to httpError has effect.
        This call will discard all data in the output pipeline queues. If some data has already been written to the client
        the connection will be aborted so the client can get some indication that an error has occurred after the
        headers have been transmitted.
    @param conn HttpConn connection object created via #httpCreateConn
    @param status Http status code. The status code can be ored with the flags HTTP_ABORT to immediately abort the connection
        or HTTP_CLOSE to close the connection at the completion of the request.
    @param fmt Printf style formatted string
    @param ... Arguments for fmt
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpError(HttpConn *conn, int status, cchar *fmt, ...);

/**
    Http I/O event handler. Invoke when there is an I/O event on the connection. This is normally invoked automatically
    when I/O events are received.
    @param conn HttpConn object created via #httpCreateConn
    @param event Event structure
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpEvent(struct HttpConn *conn, MprEvent *event);

/**
    Get the async mode value for the connection
    @param conn HttpConn object created via #httpCreateConn
    @return True if the connection is in async mode
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC int httpGetAsync(HttpConn *conn);

/**
    Get the preferred chunked size for transfer chunk encoding.
    @param conn HttpConn connection object created via #httpCreateConn
    @return Chunk size. Returns "zero" if not yet defined.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC ssize httpGetChunkSize(HttpConn *conn);

/**
    Get the connection context object
    @param conn HttpConn object created via #httpCreateConn
    @return The connection context object defined via httpSetConnContext
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void *httpGetConnContext(HttpConn *conn);

/**
    Get the connection host object
    @param conn HttpConn object created via #httpCreateConn
    @return The connection host object defined via httpSetConnHost
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void *httpGetConnHost(HttpConn *conn);

/** 
    Get the error message associated with the last request.
    @description Error messages may be generated for internal or client side errors.
    @param conn HttpConn connection object created via #httpCreateConn
    @return A error string. The caller must not free this reference.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC cchar *httpGetError(HttpConn *conn);

/**
    Get a URI extension 
    @description If the URI has no extension and the response content filename (HttpTx.filename) has been calculated,
        it will be tested for an extension.
    @param conn HttpConn connection object created via #httpCreateConn
    @return The URI extension without the leading period.
    @ingroup HttpConn
    @stability Stable
  */
PUBLIC char *httpGetExt(HttpConn *conn);

/** 
    Get the count of Keep-Alive requests that will be used for this connection object.
    @description Http Keep-Alive means that the TCP/IP connection is preserved accross multiple requests. This
        typically means much higher performance and better response. Http Keep-Alive is enabled by default 
        for Http/1.1 (the default). Disable Keep-Alive when talking to old, broken HTTP servers.
    @param conn HttpConn connection object created via #httpCreateConn
    @return The maximum count of Keep-Alive requests. 
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC int httpGetKeepAliveCount(HttpConn *conn);

/**
    Get more output data by invoking the writable callback.
    @description This optional handler callback is invoked when the service queue has room for more data.
    @param conn HttpConn object created via #httpCreateConn
    @return True if the handler writable callback exists and can be invoked.
    @ingroup HttpConn
    @stability Evolving
 */
PUBLIC bool httpGetMoreOutput(HttpConn *conn);

/** 
    Get the count of bytes buffered on the write queue.
    @param conn HttpConn connection object created via #httpCreateConn
    @return The number of bytes buffered.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC ssize httpGetWriteQueueCount(HttpConn *conn);

/**
    Match the HttpHost object that should serve this request
    @description This sets the conn->host field to the appropriate host. If no suitable host can be found, #httpError
        will be called and conn->error will be set.
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
  */
PUBLIC void httpMatchHost(HttpConn *conn);

/**
    Signal a memory allocation error
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpMemoryError(HttpConn *conn);

/**
    Inform notifiers of a connection event or state change.
    @description This is an internal API and should not be called by handler or user code.
    @param conn HttpConn object created via #httpCreateConn
    @param event Event to issue
    @param arg Argument to event
    @ingroup HttpConn
    @stability Evolving
 */ 
PUBLIC void httpNotify(HttpConn *conn, int event, int arg);

#define HTTP_NOTIFY(conn, event, arg) \
    if (1) { \
        if (conn->notifier) { \
            httpNotify(conn, event, arg); \
        } \
    } else

/**
    Do setup after an I/O event to receive future events.
    @param conn HttpConn object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpAfterEvent(HttpConn *conn);

/**
    Prepare a client connection for a new request. 
    @param conn HttpConn object created via #httpCreateConn
    @param keepHeaders If true, keep the headers already defined on the connection object
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpPrepClientConn(HttpConn *conn, bool keepHeaders);

/**
    Run the handler ready callback.
    @description This will be called when all incoming data for the request has been fully received.
    @param conn HttpConn object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpReadyHandler(HttpConn *conn);

/** 
    Reset the current security credentials
    @description Remove any existing security credentials.
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpResetCredentials(HttpConn *conn);

/**
    Route the request and select that matching route and handle to process the request.
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
  */
PUBLIC void httpRouteRequest(HttpConn *conn);

/**
    Service pipeline queues to flow data.
    @param conn HttpConn object created via #httpCreateConn
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC bool httpServiceQueues(HttpConn *conn);

/**
    Set the async mode value for the connection
    @param conn HttpConn object created via #httpCreateConn
    @param enable Set to 1 to enable async mode
    @return True if the connection is in async mode
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetAsync(HttpConn *conn, int enable);

/** 
    Set the chunk size for transfer chunked encoding. When set, a "Transfer-Encoding: Chunked" header will
    be added to the request, and all write data will be broken into chunks of the requested size.
    @param conn HttpConn connection object created via #httpCreateConn
    @param size Requested chunk size.
    @ingroup HttpConn
    @stability Stable
 */ 
PUBLIC void httpSetChunkSize(HttpConn *conn, ssize size);

/**
    Set the connection context object
    @param conn HttpConn object created via #httpCreateConn
    @param context New context object
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetConnContext(HttpConn *conn, void *context);

/**
    Set the connection host object
    @param conn HttpConn object created via #httpCreateConn
    @param host New context host
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetConnHost(HttpConn *conn, void *host);

/** 
    Define a notifier callback for this connection.
    @description The notifier callback will be invoked for state changes and I/O events as Http requests are processed.
    The supported events are:
    <ul>
    <li>HTTP_EVENT_STATE &mdash; The request is changing state. Valid states are:
        HTTP_STATE_BEGIN, HTTP_STATE_CONNECTED, HTTP_STATE_FIRST, HTTP_STATE_CONTENT, HTTP_STATE_READY,
        HTTP_STATE_RUNNING, HTTP_STATE_FINALIZED and HTTP_STATE_COMPLETE. A request will always visit all states and the
        notifier will be invoked for each and every state. This is true even if the request has no content, the
        HTTP_STATE_CONTENT will still be visited.</li>
    <li>HTTP_EVENT_READABLE &mdash; There is data available to read</li>
    <li>HTTP_EVENT_WRITABLE &mdash; The outgoing pipeline can absorb more data</li>
    <li>HTTP_EVENT_ERROR &mdash; The request has encountered an error</li>
    <li>HTTP_EVENT_DESTROY &mdash; The connection structure is about to be destoyed</li>
    <li>HTTP_EVENT_OPEN &mdash; The application layer is now open</li>
    <li>HTTP_EVENT_CLOSE &mdash; The application layer is now closed</li>
    </ul>
    @param conn HttpConn connection object created via #httpCreateConn
    @param notifier Notifier function. 
    @ingroup HttpConn
    @stability Evolving
 */
PUBLIC void httpSetConnNotifier(HttpConn *conn, HttpNotifier notifier);

/** 
    Set the Http credentials
    @description Define a user and password to use with Http authentication for sites that require it. This will
        be used for the next client connection.
    @param conn HttpConn connection object created via #httpCreateConn
    @param user String user
    @param password Decrypted password string
    @param authType Authentication type. Set to basic or digest. Defaults to nothing.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetCredentials(HttpConn *conn, cchar *user, cchar *password, cchar *authType);

/** 
    Control Http Keep-Alive for the connection.
    @description Http Keep-Alive means that the TCP/IP connection is preserved accross multiple requests. This
        typically means much higher performance and better response. Http Keep-Alive is enabled by default 
        for Http/1.1 (the default). Disable Keep-Alive when talking to old, broken HTTP servers.
    @param conn HttpConn connection object created via #httpCreateConn
    @param count Count of Keep-Alive transactions to use before closing the connection. Set to zero to disable keep-alive.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetKeepAliveCount(HttpConn *conn, int count);

/** 
    Set the Http protocol variant for this connection
    @description Set the Http protocol variant to use. 
    @param conn HttpConn connection object created via #httpCreateConn
    @param protocol  String representing the protocol variant. Valid values are: "HTTP/1.0", "HTTP/1.1". This parameter
        must be persistent.
    Use HTTP/1.1 wherever possible.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetProtocol(HttpConn *conn, cchar *protocol);

/** 
    Set the Http retry count
    @description Define the number of retries before failing a request. It is normative for network errors
        to require that requests be sometimes retried. The default retries is set to (2).
    @param conn HttpConn object created via #httpCreateConn
    @param retries Count of retries
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetRetries(HttpConn *conn, int retries);

#if !BIT_ROM
/**
    Set the "Send" connector to process the request
    @description If the net connection has been selected, but the response content is a file, the pipeline connector
    can be upgraded to use the "Send" connector.
    @param conn HttpConn connection object created via #httpCreateConn
    @param path File name to send as a response
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetSendConnector(HttpConn *conn, cchar *path);
#endif

/**
    Set the connection state and invoke notifiers.
    @description The connection states are, in order : HTTP_STATE_BEGIN HTTP_STATE_CONNECTED HTTP_STATE_FIRST
    HTTP_STATE_PARSED HTTP_STATE_CONTENT HTTP_STATE_READY HTTP_STATE_RUNNING HTTP_STATE_FINALIZED HTTP_STATE_COMPLETE. 
    When httpSetState advances the state it will invoke any registered #HttpNotifier. If the state is set to a state beyond
        the next intermediate state, the HttpNotifier will be invoked for all intervening states. 
        This is true even if the request has no content, the HTTP_STATE_CONTENT will still be visited..
    @param conn HttpConn object created via #httpCreateConn
    @param state New state to enter
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpSetState(HttpConn *conn, int state);

/** 
    Set the Http inactivity timeout
    @description Define an inactivity timeout after which the Http connection will be closed. 
    @param conn HttpConn object created via #httpCreateConn
    @param requestTimeout Request timeout in msec. This is the total time for the request. Set to -1 to preserve the
        existing value.
    @param inactivityTimeout Inactivity timeout in msec. This is maximum connection idle time. Set to -1 to preserve the
        existing value.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC void httpSetTimeout(HttpConn *conn, MprTicks requestTimeout, MprTicks inactivityTimeout);

/**
    Define a timestamp in the MPR log file.
    @description This routine initiates the writing of a timestamp in the MPR log file
    @param period Time in milliseconds between timestamps
    @ingroup HttpConn
    @stability Evolving
 */
PUBLIC void httpSetTimestamp(MprTicks period);

/**
    Setup a wait handler for the connection to wait for desired events
    @param conn HttpConn object created via #httpCreateConn
    @param eventMask Mask of events. MPR_READABLE | MPR_WRITABLE
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpSetupWaitHandler(HttpConn *conn, int eventMask);

/**
    Test if the item should be traced
    @param conn HttpConn connection object created via #httpCreateConn
    @param dir Direction of data flow. Set to HTTP_TRACE_RX or HTTP_TRACE_TX
    @param item Item to trace. Set to HTTP_TRACE_CONN, HTTP_TRACE_FIRST, HTTP_TRACE_HEADER, HTTP_TRACE_BODY, or 
        HTTP_TRACE_TIME
    @param ext URI resource extension (without ".").
    @return The level at which tracing should be done. Returns -1 if tracing should not be done for this item.
    @ingroup HttpConn
    @stability Internal
  */
PUBLIC int httpShouldTrace(HttpConn *conn, int dir, int item, cchar *ext);

/**
    Start the pipeline. This starts the request handler.
    @param conn HttpConn object created via #httpCreateConn
    @ingroup HttpConn
    @stability Internal
 */
PUBLIC void httpStartPipeline(HttpConn *conn);

/**
    Steal a connection from Appweb
    @description Steal a connection from Appweb and assume total responsibility for the connection.
    This removes the connection from active management by Appweb. After calling, request and inactivity
    timeouts will not be enforced. It is the callers responsibility to call mprCloseSocket.
    @param conn HttpConn object created via #httpCreateConn
    @return The connection socket object.
    @ingroup HttpConn
    @stability Evolving
 */
PUBLIC MprSocket *httpStealConn(HttpConn *conn);

/**
    Verify the server handshake
    @param conn HttpConn connection object created via #httpCreateConn
    @return True if the handshake is valid
    @ingroup HttpConn
    @stability Evolving
 */
PUBLIC bool httpVerifyWebSocketsHandshake(HttpConn *conn);

/* Internal APIs */
PUBLIC struct HttpConn *httpAccept(struct HttpEndpoint *endpoint);
PUBLIC void httpEnableConnEvents(HttpConn *conn);
PUBLIC void httpInitTrace(HttpTrace *trace);
PUBLIC void httpParseMethod(HttpConn *conn);
PUBLIC HttpLimits *httpSetUniqueConnLimits(HttpConn *conn);
PUBLIC int httpShouldTrace(HttpConn *conn, int dir, int item, cchar *ext);
PUBLIC void httpTraceContent(HttpConn *conn, int dir, int item, HttpPacket *packet, ssize len, MprOff total);
PUBLIC void httpUsePrimary(HttpConn *conn);
PUBLIC void httpUseWorker(HttpConn *conn, MprDispatcher *dispatcher, MprEvent *event);

/********************************** HttpAuth *********************************/
/*  
    Authorization flags for HttpAuth.flags
 */
#define HTTP_ALLOW_DENY     0x1           /**< Run allow checks before deny checks */
#define HTTP_DENY_ALLOW     0x2           /**< Run deny checks before allow checks */
#define HTTP_AUTO_LOGIN     0x4           /**< Auto login for debug */

/**
    AuthType callback to generate a response requesting the user login
    This should call httpError if such a response cannot be generated.
    @param conn HttpConn connection object 
    @ingroup HttpAuth
    @stability Evolving
 */
typedef void (*HttpAskLogin)(HttpConn *conn);

/**
    AuthType callback to parse the HTTP 'Authorize' (client) and 'www-authenticate' (server) headers
    @param conn HttpConn connection object 
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
 */
typedef int (*HttpParseAuth)(HttpConn *conn);

/**
    AuthType callback to set the necessary HTTP authorization headers for a client request
    @param conn HttpConn connection object 
    @return True if the authorization headers can be set. 
    @ingroup HttpAuth
    @stability Evolving
 */
typedef bool (*HttpSetAuth)(HttpConn *conn);

/**
    AuthStore callback Verify the user credentials
    @param conn HttpConn connection object 
    @return True if the user credentials can validate
    @ingroup HttpAuth
    @stability Evolving
 */
typedef bool (*HttpVerifyUser)(HttpConn *conn);


/**
    Authentication Protocol. Supported protocols  are: basic, digest, form.
    @ingroup HttpAuth
    @stability Internal
 */
typedef struct HttpAuthType {
    char            *name;          /**< Authentication protocol name: 'basic', 'digest', 'post' */
    HttpAskLogin    askLogin;       /**< Callback to generate a client login response */
    HttpParseAuth   parseAuth;      /**< Callback to parse the HTTP authentication headers */
    HttpSetAuth     setAuth;        /**< Callback to set the HTTP response authentication headers */
} HttpAuthType;


/**
    Password backend store. Support stores are: system, file
    @ingroup HttpAuth
    @stability Internal
 */
typedef struct HttpAuthStore {
    char            *name;          /**< Authentication password store name: 'system', 'file' */
    HttpVerifyUser  verifyUser;
} HttpAuthStore;

/** 
    User Authorization. A user has a name, password and a set of roles. These roles define a set of abilities.
    @see HttpAuth
    @ingroup HttpAuth
    @stability Internal
 */
typedef struct HttpUser {
    char            *name;                  /**< User name */
    char            *password;              /**< User password */
    char            *roles;                 /**< Original list of roles */
    MprHash         *abilities;             /**< User abilities */
} HttpUser;

/** 
    Authorization Roles. Roles are named sets of abilities.
    @see HttpAuth
    @ingroup HttpAuth
    @stability Internal
 */
typedef struct  HttpRole {
    char            *name;                  /**< Role name */
    MprHash         *abilities;             /**< Role's abilities */
} HttpRole;

/** 
    Authorization
    @description HttpAuth is the foundation authorization object and is used by HttpRoute.
    It stores the authorization configuration information required to determine if a client request should be permitted 
    access to a given resource.
    @defgroup HttpAuth HttpAuth
    @see HttpAskLogin HttpAuth HttpAuthStore HttpAuthType HttpParseAuth HttpRole HttpSetAuth HttpVerifyUser HttpUser
        HttpVerifyUser httpAddAuthType httpAddAuthStore httpAddRole httpAddUser httpCanUser httpAuthenticate
        httpComputeAllUserAbilities httpComputeUserAbilities httpCreateRole httpCreateAuth httpCreateUser
        httpIsAuthenticated httpLogin httpRemoveRole httpRemoveUser httpSetAuthAllow httpSetAuthAnyValidUser
        httpSetAuthAutoLogin httpSetAuthDeny httpSetAuthOrder httpSetAuthPermittedUsers httpSetAuthForm httpSetAuthQop
        httpSetAuthRealm httpSetAuthRequiredAbilities httpSetAuthStore httpSetAuthType
    @stability Internal
 */
typedef struct HttpAuth {
    struct HttpAuth *parent;                /**< Parent auth */
    char            *realm;                 /**< Realm of access */
    int             flags;                  /**< Authorization flags */
    int             version;                /**< Inherited from parent and incremented on changes */
    MprHash         *allow;                 /**< Clients to allow */
    MprHash         *deny;                  /**< Clients to deny */
    MprHash         *users;                 /**< Hash of users */
    MprHash         *roles;                 /**< Hash of roles */
    MprHash         *requiredAbilities;     /**< Set of required abilities (all are required) */
    MprHash         *permittedUsers;        /**< User name for access */
    char            *loginPage;             /**< Web page for user login for 'post' type */
    char            *loggedIn;              /**< Target URI after logging in */
    char            *qop;                   /**< Quality of service */
    HttpAuthType    *type;                  /**< Authorization protocol type (basic|digest|form|custom)*/
    HttpAuthStore   *store;                 /**< Authorization password backend (system|file|custom)*/
} HttpAuth;


/**
    Add an authorization type. The pre-supplied types are 'basic', 'digest' and 'post'.
    @description This creates an AuthType object with the defined name and callbacks.
    @param http Http service object.
    @param name Unique authorization type name
    @param askLogin Callback to generate a client login response
    @param parse Callback to parse the HTTP authentication headers
    @param setAuth Callback to set the HTTP response authentication headers
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC int httpAddAuthType(Http *http, cchar *name, HttpAskLogin askLogin, HttpParseAuth parse, HttpSetAuth setAuth);

/**
    Add an authorization store for password validation. The pre-supplied types are 'system' and 'file'
    @description This creates an AuthType object with the defined name and callbacks.
    @param http Http service object.
    @param name Unique authorization type name
    @param verifyUser Callback to verify the username and password contained in the HttpConn object passed to the callback.
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC int httpAddAuthStore(Http *http, cchar *name, HttpVerifyUser verifyUser);

/**
    Add a role
    @description This creates the role with given abilities. Ability words can also be other roles.
    @param auth Auth object allocated by #httpCreateAuth.
    @param role Role name to add
    @param abilities Space separated list of abilities.
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC int httpAddRole(HttpAuth *auth, cchar *role, cchar *abilities);

/**
    Add a user
    @description This creates the user and adds the user to the authentication database.
    @param auth Auth object allocated by #httpCreateAuth.
    @param user User name to add
    @param password User password. The password should not be encrypted. The backend will encrypt as required.
    @param abilities Space separated list of abilities.
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC int httpAddUser(HttpAuth *auth, cchar *user, cchar *password, cchar *abilities);

/**
    Test if a user has the required abilities
    @param conn HttpConn connection object created via #httpCreateConn object.
    @return True if the user has all the required abilities
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC bool httpCanUser(HttpConn *conn);

/**
    Compute all the user abilities for a route using the given auth
    @param auth Auth object allocated by #httpCreateAuth
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpComputeAllUserAbilities(HttpAuth *auth);

/**
    Compute the user abilities for a given user in a route using the given auth
    @param auth Auth object allocated by #httpCreateAuth
    @param user User object
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpComputeUserAbilities(HttpAuth *auth, HttpUser *user);

/**
    Create an authentication object
    @return An empty authentiction object
    @ingroup HttpAuth
    @stability Evolving
    @internal 
 */
PUBLIC HttpAuth *httpCreateAuth();

/**
    Create a new role
    @description The role is not added to the authentication database
    @param auth Auth object allocated by #httpCreateAuth.
    @param name Role name 
    @param abilities Space separated list of abilities.
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC HttpRole *httpCreateRole(HttpAuth *auth, cchar *name, cchar *abilities);

/**
    Create a new user
    @description The user is not added to the authentication database
    @param auth Auth object allocated by #httpCreateAuth.
    @param name User name 
    @param password User password. The password should not be encrypted. The backend will encrypt as required.
    @param abilities Space separated list of abilities.
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC HttpUser *httpCreateUser(HttpAuth *auth, cchar *name, cchar *password, cchar *abilities);

/**
    Test if the user is authenticated
    @param conn HttpConn connection object 
    @return True if the username and password have been authenticated and the user has the abilities required
        to access the requested resource document.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC bool httpIsAuthenticated(HttpConn *conn);

/**
    Log the user in. 
    @description This will verify the supplied username and password. If the user is successfully logged in, 
    the user identity will be stored in session state for fast authentication on subsequent requests.
    Note: this does not verify any user abilities.
    @param conn HttpConn connection object 
    @param username User name to authenticate
    @param password Password for the user
    @return True if the username and password have been authenticated.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC bool httpLogin(HttpConn *conn, cchar *username, cchar *password);

/**
    Remove a role
    @param auth Auth object allocated by #httpCreateAuth.
    @param role Role name to remove
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
    @internal 
 */
PUBLIC int httpRemoveRole(HttpAuth *auth, cchar *role);

/**
    Remove a user
    @param auth Auth object allocated by #httpCreateAuth.
    @param user User name to remove
    @return Zero if successful, otherwise a negative MPR error code
    @ingroup HttpAuth
    @stability Evolving
    @internal 
 */
PUBLIC int httpRemoveUser(HttpAuth *auth, cchar *user);

/**
    Allow access by a client IP IP address
    @param auth Authorization object allocated by #httpCreateAuth.
    @param ip Client IP address to allow.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthAllow(HttpAuth *auth, cchar *ip);

/**
    Allow access by any valid user
    @description This configures the basic or digest authentication for the authorization object
    @param auth Authorization object allocated by #httpCreateAuth.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthAnyValidUser(HttpAuth *auth);

/**
    Enable auto login
    @description If auto-login is enabled, access will be granted to all resources.
    @param auth Auth object allocated by #httpCreateAuth.
    @param on Set to true to enable auto login.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthAutoLogin(HttpAuth *auth, bool on);

/**
    Deny access by a client IP address
    @param auth Authorization object allocated by #httpCreateAuth.
    @param ip Client IP address to deny. This must be an IP address string.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthDeny(HttpAuth *auth, cchar *ip);

/**
    Set the auth allow/deny order
    @param auth Auth object allocated by #httpCreateAuth.
    @param order Set to HTTP_ALLOW_DENY to run allow checks before deny checks. Set to HTTP_DENY_ALLOW to run deny
        checks before allow.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthOrder(HttpAuth *auth, int order);

/**
    Define the set of permitted users 
    @param auth Auth object allocated by #httpCreateAuth.
    @param users Space separated list of acceptable users.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthPermittedUsers(HttpAuth *auth, cchar *users);

/**
    Define the callbabcks for the 'post' authentication type.
    @description This creates a new route for the login page.
    @param parent Parent route from which to inherit when creating a route for the login page.
    @param loginPage Web page URI for the user to enter username and password.
    @param loginService URI to use for the internal login service. To use your own login URI, set to this the empty string. 
    @param logoutService URI to use to log the user out. To use your won logout URI, set this to the empty string.
    @param loggedIn The client is redirected to this URI once logged in. Use a "referrer:" prefix to the URI to 
        redirect the user to the referring URI before the loginPage. If the referrer cannot be determined, the base
        URI is utilized.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthForm(struct HttpRoute *parent, cchar *loginPage, cchar *loginService, cchar *logoutService, 
    cchar *loggedIn);

/**
    Set the required quality of service for digest authentication
    @description This configures the basic or digest authentication for the auth object
    @param auth Auth object allocated by #httpCreateAuth.
    @param qop Quality of service description.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthQop(HttpAuth *auth, cchar *qop);

/**
    Set the required realm for basic or digest authentication
    @description This configures the authentication realm. The realm is displayed to the user in the browser login
        dialog box.
    @param auth Auth object allocated by #httpCreateAuth.
    @param realm Authentication realm
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthRealm(HttpAuth *auth, cchar *realm);

/**
    Set the required abilities for access
    @param auth Auth object allocated by #httpCreateAuth.
    @param abilities Spaces separated list of all the required abilities.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC void httpSetAuthRequiredAbilities(HttpAuth *auth, cchar *abilities);

/**
    Set the authentication password store to use
    @param auth Auth object allocated by #httpCreateAuth.
    @param store Password store to use. Select from: 'system', 'file'
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC int httpSetAuthStore(HttpAuth *auth, cchar *store);

/**
    Set the authentication protocol to use
    @param auth Auth object allocated by #httpCreateAuth.
    @param proto Protocol name to use. Select from: 'basic', 'digest', 'form'
    @param details Extra protocol details.
    @ingroup HttpAuth
    @stability Evolving
 */
PUBLIC int httpSetAuthType(HttpAuth *auth, cchar *proto, cchar *details);

/*
    Internal
 */
PUBLIC void httpBasicLogin(HttpConn *conn);
PUBLIC int httpBasicParse(HttpConn *conn);
PUBLIC bool httpBasicSetHeaders(HttpConn *conn);
PUBLIC void httpDigestLogin(HttpConn *conn);
PUBLIC int httpDigestParse(HttpConn *conn);
PUBLIC bool httpDigestSetHeaders(HttpConn *conn);
PUBLIC bool httpPamVerifyUser(HttpConn *conn);
PUBLIC bool httpInternalVerifyUser(HttpConn *conn);
PUBLIC int httpAuthenticate(HttpConn *conn);
PUBLIC void httpComputeAllUserAbilities(HttpAuth *auth);
PUBLIC void httpComputeUserAbilities(HttpAuth *auth, HttpUser *user);
PUBLIC void httpInitAuth(Http *http);
PUBLIC HttpAuth *httpCreateInheritedAuth(HttpAuth *parent);
PUBLIC HttpAuthType *httpLookupAuthType(cchar *type);

/********************************** HttpLang  ********************************/

#define HTTP_LANG_BEFORE        0x1         /**< Insert suffix before extension */
#define HTTP_LANG_AFTER         0x2         /**< Insert suffix after extension */

/**
    Language definition record for routes
    @ingroup HttpRoute
    @stability Internal
  */
typedef struct HttpLang {
    char        *path;                      /**< Document directory for the language */
    char        *suffix;                    /**< Suffix to add to filenames */
    int         flags;                      /**< Control suffix position */
} HttpLang;

/********************************** HttpCache  *********************************/

#define HTTP_CACHE_CLIENT           0x1     /**< Cache on the client side */
#define HTTP_CACHE_SERVER           0x2     /**< Cache on the server side */
#define HTTP_CACHE_MANUAL           0x4     /**< Cache manually. User must call httpWriteCache */
#define HTTP_CACHE_RESET            0x8     /**< Don't inherit cache config from outer routes */
#define HTTP_CACHE_ALL              0x10    /**< Cache the same pathInfo together regardless of the request params */
#define HTTP_CACHE_ONLY             0x20    /**< Cache exactly the specified URI with params */
#define HTTP_CACHE_UNIQUE           0x40    /**< Uniquely cache request with different params */

/**
    Cache Control
    @defgroup HttpCache HttpCache
    @see HttpCache httpAddCache httpUpdateCache httpWriteCache
    @stability Internal
*/
typedef struct HttpCache {
    MprHash     *extensions;                /**< Extensions to cache */
    MprHash     *methods;                   /**< Methods to cache */
    MprHash     *types;                     /**< MimeTypes to cache */
    MprHash     *uris;                      /**< URIs to cache */
    MprTicks    clientLifespan;             /**< Lifespan for client cached content */
    MprTicks    serverLifespan;             /**< Lifespan for server cached content */
    int         flags;                      /**< Cache control flags */
} HttpCache;

/**
    Add caching for response content
    @description This call configures caching for request responses. Caching may be used for any HTTP method, 
    though typically it is most useful for state-less GET requests. Output data may be uniquely cached for requests 
    with different request parameters (query, post, and route parameters).
    \n\n
    When server-side caching is requested and manual-mode is not enabled, the request response will be automatically 
    cached. Subsequent client requests will revalidate the cached content with the server. If the server-side cached 
    content has not expired, a HTTP Not-Modified (304) response will be sent and the client will use its client-side 
    cached content.  This results in a very fast transaction with the client as no response data is sent.
    Server-side caching will cache both the response headers and content.
    \n\n
    If manual server-side caching is requested, the response will be automatically cached, but subsequent requests will
    require the handler to explicitly send cached content by calling #httpWriteCached.
    \n\n
    If client-side caching is requested, a "Cache-Control" Http header will be sent to the client with the caching 
    "max-age" set to the lifespan argument value (converted to seconds). This causes the client to serve client-cached 
    content and to not contact the server at all until the max-age expires. 
    Alternatively, you can use #httpSetHeader to explicitly set a "Cache-Control header. For your reference, here are 
    some keywords that can be used in the Cache-Control Http header.
    \n\n
        "max-age" Maximum time in seconds the resource is considered fresh.
        "s-maxage" Maximum time in seconds the resource is considered fresh from a shared cache.
        "public" marks authenticated responses as cacheable.
        "private" shared caches may not store the response.
        "no-cache" cache must re-submit request for validation before using cached copy.
        "no-store" response may not be stored in a cache.
        "must-revalidate" forces clients to revalidate the request with the server.
        "proxy-revalidate" similar to must-revalidate except only for proxy caches.
    \n\n
    Use client-side caching for static content that will rarely change or for content for which using "reload" in 
    the browser is an adequate solution to force a refresh. Use manual server-side caching for situations where you need to
    explicitly control when and how cached data is returned to the client. For most other situations, use server-side
    caching.
    @param route HttpRoute object
    @param methods List of methods for which caching should be enabled. Set to a comma or space separated list
        of method names. Method names can be any case. Set to null or "*" for all methods. Example:
        "GET, POST".
    @param uris Set of URIs to cache. 
        If the URI is set to "*" all URIs for that action are uniquely cached. If the request has POST data, 
        the URI may include such post data in a sorted query format. E.g. {uri: /buy?item=scarf&quantity=1}.
    @param extensions List of document extensions for which caching should be enabled. Set to a comma or space 
        separated list of extensions. Extensions should not have a period prefix. Set to null or "*" for all extensions.
        Example: "html, css, js".
    @param types List of document mime types for which caching should be enabled. Set to a comma or space 
        separated list of types. The mime types are those that correspond to the document extension and NOT the
        content type defined by the handler serving the document. Set to null or "*" for all types.
        Example: "image/gif, application/x-php".
    @param clientLifespan Lifespan of client cache items in milliseconds. If not set to positive integer, the lifespan will
        default to the route lifespan.
    @param serverLifespan Lifespan of server cache items in milliseconds. If not set to positive integer, the lifespan will
        default to the route lifespan.
    @param flags Cache control flags. Select HTTP_CACHE_MANUAL to enable manual mode. In manual mode, cached content
        will not be automatically sent. Use #httpWriteCached in the request handler to write previously cached content.
        \n\n
        Select HTTP_CACHE_CLIENT to enable client-side caching. In this mode a "Cache-Control" Http header will be 
        sent to the client with the caching "max-age". WARNING: the client will not send any request for this URI
        until the max-age timeout has expired.
        \n\n
        Select HTTP_CACHE_RESET to first reset existing caching configuration for this route.
        \n\n
        Select HTTP_CACHE_ALL, HTTP_CACHE_ONLY or HTTP_CACHE_UNIQUE to define the server-side caching mode. Only
        one of these three mode flags should be specified.
        \n\n
        If the HTTP_CACHE_ALL flag is set, the request params (query, post data, and route parameters) will be
        ignored and all requests for a given URI path will cache to the same cache record.
        \n\n
        Select HTTP_CACHE_UNIQUE to uniquely cache requests with different request parameters. The URIs specified in 
        uris should not contain any request parameters.
        \n\n
        Select HTTP_CACHE_ONLY to cache only the exact URI with parameters specified in uris. The parameters must be 
        in sorted www-urlencoded format. For example: /example.esp?hobby=sailing&name=john.
    @return A count of the bytes actually written
    @ingroup HttpCache
    @stability Evolving
 */
PUBLIC void httpAddCache(struct HttpRoute *route, cchar *methods, cchar *uris, cchar *extensions, cchar *types, 
        MprTicks clientLifespan, MprTicks serverLifespan, int flags);

/**
    Update the cached content for a URI
    @param conn HttpConn connection object 
    @param uri The request URI for which to update the cache. If using the HTTP_CACHE_ONLY flag when configuring 
        the cached item, then the URI should contain the request parameters in sorted www-urlencoded format.
    @param data Data to cache for the URI. If you wish to cache response headers, include those at the start of the
    data followed by an additional new line. For example: "Content-Type: text/plain\n\nHello World\n".
    @param lifespan Lifespan in milliseconds for the cached content
    @ingroup HttpCache
    @stability Evolving
  */
PUBLIC ssize httpUpdateCache(HttpConn *conn, cchar *uri, cchar *data, MprTicks lifespan);

/**
    Write the cached content for a URI to the client
    @description This call explicitly writes cached content to the client. It is useful when the caching is 
        configured in manual mode via the HTTP_CACHE_MANUAL flag to #httpAddCache.
    @param conn HttpConn connection object 
    @ingroup HttpCache
    @stability Evolving
  */
PUBLIC ssize httpWriteCached(HttpConn *conn);

/******************************** Action Handler *************************************/
/**
    Action handler callback signature
    @description The Action Handler provides a simple mechanism to bind "C" callback functions with URIs.
    @param conn HttpConn connection object created via #httpCreateConn
    @defgroup HttpConn HttpConn
    @stability Evolving
 */
typedef void (*HttpAction)(HttpConn *conn);

/**
    Define a function procedure to invoke when the specified URI is requested.
    @description This creates the role with given abilities. Ability words can also be other roles.
    @param uri URI to bind with. When this URI is requested, the callback will be invoked if the procHandler is 
        configured for the request route.
    @param fun Callback function procedure
    @ingroup HttpAction
    @stability Evolving
 */
PUBLIC void httpDefineAction(cchar *uri, HttpAction fun);

/********************************** HttpRoute  *********************************/
/*
    Misc route API flags
 */
#define HTTP_ROUTE_NOT                  0x1         /**< Negate the route pattern test result */
#define HTTP_ROUTE_FREE                 0x2         /**< Free Route.mdata back to malloc when route is freed */
#define HTTP_ROUTE_FREE_PATTERN         0x4         /**< Free Route.patternCompiled back to malloc when route is freed */
#define HTTP_ROUTE_RAW                  0x8         /**< Don't html encode the write data */
#define HTTP_ROUTE_GZIP                 0x1000      /**< Support gzipped content on this route */
#define HTTP_ROUTE_STARTED              0x2000      /**< Route initialized */
#define HTTP_ROUTE_PUT_DELETE_METHODS   0x4000      /**< Support PUT|DELETE on this route */
#define HTTP_ROUTE_TRACE_METHOD         0x8000      /**< Enable the trace method for handlers supporting it */

/**
    Route Control
    @defgroup HttpRoute HttpRoute
    @see HttpRoute httpAddRouteCondition httpAddRouteErrorDocument
        httpAddRouteFilter httpAddRouteHandler httpAddRouteHeader httpAddRouteLanguageDir httpAddRouteLanguageSuffix 
        httpAddRouteLoad httpAddRouteQuery httpAddRouteUpdate httpClearRouteStages httpCreateAliasRoute 
        httpCreateDefaultRoute httpCreateInheritedRoute httpCreateRoute httpDefineRoute
        httpDefineRouteCondition httpDefineRouteTarget httpDefineRouteUpdate httpFinalizeRoute httpGetRouteData 
        httpGetRouteDir httpLink httpLookupRouteErrorDocument httpMakePath httpMatchRoute httpResetRoutePipeline 
        httpSetRouteAuth httpSetRouteAutoDelete httpSetRouteCompression httpSetRouteConnector httpSetRouteData 
        httpSetRouteDefaultLanguage httpSetRouteDir httpSetRouteFlags httpSetRouteHandler httpSetRouteHost 
        httpSetRouteIndex httpSetRouteMethods httpSetRouteName httpSetRouteVar httpSetRoutePattern 
        httpSetRoutePrefix httpSetRouteScript httpSetRouteSource httpSetRouteTarget httpSetRouteWorkers httpTemplate 
        httpSetTrace httpSetTraceFilter httpTokenize httpTokenizev 
    @stability Internal
 */
typedef struct HttpRoute {
    /* Ordered for debugging */
    struct HttpRoute *parent;               /**< Parent route */
    char            *name;                  /**< Route name */
    char            *pattern;               /**< Original matching URI pattern for the route (includes prefix) */
    char            *startSegment;          /**< Starting literal segment of pattern (includes prefix) */
    char            *startWith;             /**< Starting literal segment of pattern (includes prefix) */
    char            *optimizedPattern;      /**< Processed pattern (excludes prefix) */
    char            *prefix;                /**< Application scriptName prefix */
    char            *tplate;                /**< URI template for forming links based on this route (includes prefix) */
    char            *targetRule;            /**< Target rule */
    char            *target;                /**< Route target details */

    //  MOB - rename documents
    char            *dir;                   /**< Documents directory */
    char            *home;                  /**< Home directory for configuration files */
    MprList         *indicies;              /**< Directory index documents */
    char            *methodSpec;            /**< Supported HTTP methods */
    HttpStage       *handler;               /**< Fixed handler */

    int             nextGroup;              /**< Next route with a different startWith */
    int             responseStatus;         /**< Response status code */
    ssize           prefixLen;              /**< Prefix length */
    ssize           startWithLen;           /**< Length of startWith */
    ssize           startSegmentLen;        /**< Prefix length */

    MprList         *caching;               /**< Items to cache */
    MprTicks        lifespan;               /**< Default lifespan for all cache items in route */
    HttpAuth        *auth;                  /**< Per route block authentication */
    Http            *http;                  /**< Http service object (copy of appweb->http) */
    struct HttpHost *host;                  /**< Owning host */
    int             flags;                  /**< Route flags */

    char            *defaultLanguage;       /**< Default language */
    MprHash         *extensions;            /**< Hash of handlers by extensions */
    MprList         *handlers;              /**< List of handlers for this route */
    HttpStage       *connector;             /**< Network connector to use */
    MprHash         *data;                  /**< Hash of extra data configuration */
    MprHash         *vars;                  /**< Route variables. Used to expand Path ${token} refrerences */
    MprHash         *languages;             /**< Languages supported */
    MprList         *inputStages;           /**< Input stages */
    MprList         *outputStages;          /**< Output stages */
    MprHash         *errorDocuments;        /**< Set of error documents to use on errors */
    void            *context;               /**< Hosting context (Appweb == EjsPool) */
    void            *eroute;                /**< Extended route information for handler (only) */
    char            *uploadDir;             /**< Upload directory */
    int             autoDelete;             /**< Automatically delete uploaded files */

    MprFile         *log;                   /**< File object for access logging */
    char            *logFormat;             /**< Access log format */
    char            *logPath;               /**< Access log filename */
    int             logFlags;               /**< Log control flags (append|anew) */
    int             logBackup;              /**< Number of log backups */
    ssize           logSize;                /**< Max log size */
    HttpLimits      *limits;                /**< Host resource limits */
    MprHash         *mimeTypes;             /**< Hash table of mime types (key is extension) */

    HttpTrace       trace[2];               /**< Default route request tracing */
    int             traceMask;              /**< Request/response trace mask */

    /*
        Used by Ejscript
     */
    char            *script;                /**< Startup script for handlers serving this route */
    char            *scriptPath;            /**< Startup script path for handlers serving this route */
    int             workers;                /**< Number of workers to use for this route */

    MprHash         *methods;               /**< Matching HTTP methods */
    MprList         *params;                /**< Matching param field data */
    MprList         *headers;               /**< Matching header values */
    MprList         *conditions;            /**< Route conditions */
    MprList         *updates;               /**< Route and request updates */

    void            *patternCompiled;       /**< Compiled pattern regular expression (not alloced) */
    char            *sourceName;            /**< Source name for route target */
    MprList         *tokens;                /**< Tokens in pattern, {name} */

    struct MprSsl   *ssl;                   /**< SSL configuration */
    MprMutex        *mutex;                 /**< Multithread sync */

    char            *webSocketsProtocol;    /**< WebSockets sub-protocol */
    MprTicks        webSocketsPingPeriod;   /**< Time between pings (msec) */
    int             ignoreEncodingErrors;   /**< Ignore UTF8 encoding errors */
} HttpRoute;


/**
    Route operation record
    @stability Internal
 */
typedef struct HttpRouteOp {
    char            *name;                  /**< Name of route operation */
    char            *details;               /**< General route operation details */
    char            *var;                   /**< Var to set */
    char            *value;                 /**< Value to assign to var */
    void            *mdata;                 /**< pcre_ data managed by malloc() */
    int             flags;                  /**< Route flags to control freeing mdata */
} HttpRouteOp;

/*
    Route matching return codes
 */
#define HTTP_ROUTE_OK       0             /**< The route matches the request */
#define HTTP_ROUTE_REJECT   1             /**< The route does not match the request */
#define HTTP_ROUTE_REROUTE  2             /**< Request has been modified and must be re-routed */

/**
    General route procedure. Used by targets, conditions and updates.
 */
typedef int (HttpRouteProc)(HttpConn *conn, HttpRoute *route, HttpRouteOp *item);

/**
    Add routes for a resource
    @description This routing adds a set of RESTful routes for a resource. It will add the following routes:
    <table>
        <tr><td>Name</td><td>Method</td><td>Pattern</td><td>Action</td></tr>
        <tr><td>init</td><td>GET</td><td>/NAME/init$</td><td>init</td></tr>
        <tr><td>create</td><td>POST</td><td>/NAME(/)*$</td><td>create</td></tr>
        <tr><td>edit</td><td>GET</td><td>/NAME/edit$</td><td>edit</td></tr>
        <tr><td>show</td><td>GET</td><td>/NAME$</td><td>show</td></tr>
        <tr><td>update</td><td>PUT</td><td>/NAME$</td><td>update</td></tr>
        <tr><td>destroy</td><td>DELETE</td><td>/NAME$</td><td>destroy</td></tr>
        <tr><td>default</td><td>*</td><td>/NAME/{action}$</td><td>cmd-${action}</td></tr>
    </tr>
    </table>
    @param parent Parent route from which to inherit configuration.
    @param resource Resource name. This should be a lower case, single word, alphabetic resource name.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddResource(HttpRoute *parent, cchar *resource);

/**
    Add routes for a group of resources
    @description This routing adds a set of RESTful routes for a resource group. It will add the following routes:
    <table>
        <tr><td>Name</td><td>Method</td><td>Pattern</td><td>Action</td></tr>
        <tr><td>list</td><td>GET</td><td>/NAME(/)*$</td><td>list</td></tr>
        <tr><td>init</td><td>GET</td><td>/NAME/init$</td><td>init</td></tr>
        <tr><td>create</td><td>POST</td><td>/NAME(/)*$</td><td>create</td></tr>
        <tr><td>edit</td><td>GET</td><td>/NAME/{id=[0-9]+}/edit$</td><td>edit</td></tr>
        <tr><td>show</td><td>GET</td><td>/NAME/{id=[0-9]+}$</td><td>show</td></tr>
        <tr><td>update</td><td>PUT</td><td>/NAME/{id=[0-9]+}$</td><td>update</td></tr>
        <tr><td>destroy</td><td>DELETE</td><td>/NAME/{id=[0-9]+}$</td><td>destroy</td></tr>
        <tr><td>custom</td><td>POST</td><td>/NAME/{action}/{id=[0-9]+}$</td><td>${action}</td></tr>
        <tr><td>default</td><td>*</td><td>/NAME/{action}$</td><td>cmd-${action}</td></tr>
    </tr>
    </table>
    @param parent Parent route from which to inherit configuration.
    @param resource Resource name. This should be a lower case, single word, alphabetic resource name.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddResourceGroup(HttpRoute *parent, cchar *resource);

/**
    Add a route for the home page.
    @description This will add a home page to route ESP applications. This will add the following route:
    <table>
        <tr><td>Name</td><td>Method</td><td>Pattern</td><td>Target</td></tr>
        <tr><td>home</td><td>GET,POST,PUT</td><td>^/$</td><td>index.esp</td></tr>
    </table>
    @param parent Parent route from which to inherit configuration.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddHomeRoute(HttpRoute *parent);

/**
    Add a route set package
    @description This will add a set of routes suitable for some application paradigms.
    <table>
        <tr><td>Name</td><td>Method</td><td>Pattern</td><td>Target</td></tr>
        <tr><td>home</td><td>GET,POST,PUT</td><td>^/$</td><td>index.esp</td></tr>
        <tr><td>static</td><td>GET</td><td>^/static(/(.)*$</td><td>$1</td></tr>
    </table>
    @param parent Parent route from which to inherit configuration.
    @param set Route set to select. Use "simple", "mvc", "restful" or "none". 
        \n\n
        The "simple" pack will invoke 
        #httpAddHomeRoute and #httpAddStaticRoute to add "home", and "static" routes. 
        \n\n
        The "mvc" selection will add the default routes and then add the route:
        <table>
            <tr><td>Name</td><td>Method</td><td>Pattern</td><td>Target</td></tr>
            <tr><td>default</td><td>*</td><td>^/{controller}(~/{action}~)$</td><td>${controller}-${action}</td></tr>
        </table>
        \n\n
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddRouteSet(HttpRoute *parent, cchar *set);

/**
    Add a route condition
    @description A route condition is run after matching the route pattern. For a route to be accepted, all conditions
        must match. Route conditions are built-in rules that can be applied to routes.
    @param route Route to modify
    @param name Condition rule to add. Supported conditions are: "auth", "missing", "directory", "exists", and "match".
        The "auth" rule is used internally to implement basic and digest authentication. 
        \n\n
        The "missing" rule tests if the target filename is missing. The "missing" rule takes no arguments. 
        \n\n
        The "directory" rule tests if the condition argument is a directory. The form of the "directory" rule is: 
            "directory pathString". For example: "directory /stuff/${request:pathInfo}.txt"
        \n\n
        The "exists" rule tests if the condition argument is present in the file system. The form of the "exists" rule is:
            "exists pathString". For example: "exists ${request.filename}.gz", 
        \n\n
        The match directory tests a regular expression pattern against the rest of the condition arguments. The form of 
        the match rule is: "match RegExp string". For example: "match https ${request.scheme}".
    @param details Condition parameters. 
        See #httpSetRouteTarget for a list of the token values that can be included in the condition rule details.
    @param flags Set to HTTP_ROUTE_NOT to negate the condition test
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpAddRouteCondition(HttpRoute *route, cchar *name, cchar *details, int flags);

/**
    Add an error document
    @description This defines an error document to be used when the requested document cannot be found. 
        This definition is used by some handlers for error processing.
    @param route Route to modify
    @param status The HTTP status code to use with the error document.
    @param uri URL describing the error document
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddRouteErrorDocument(HttpRoute *route, int status, cchar *uri);

/**
    Add a route filter
    @description This configures the route pipeline by adding processing filters for a request.
        must match. Route conditions are built-in rules that can be applied to routes.
    @param route Route to modify
    @param name Filter name to add
    @param extensions Request extensions for which the filter will be run. A request extension may come from the URI
        if present or from the corresponding filename.
    @param direction Set to HTTP_STAGE_TX for transmit direction and HTTP_STAGE_RX for receive data flow.
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpAddRouteFilter(HttpRoute *route, cchar *name, cchar *extensions, int direction);

/**
    Add a route handler
    @description This configures the route pipeline by adding the given handler.
        Must only be called at initialization time for the route.
    @param route Route to modify
    @param name Filter name to add
    @param extensions Request extensions for which the handler will be selected. A request extension may come from the URI
        if present or from the corresponding filename.
    @return Zero if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpAddRouteHandler(HttpRoute *route, cchar *name, cchar *extensions);

/**
    Add a route header check
    @description This configures the route to match a request only if the specified header field matches a specific value.
    @param route Route to modify
    @param header Header field to interrogate
    @param value Header value that will match
    @param flags Set to HTTP_ROUTE_NOT to negate the header test
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddRouteHeader(HttpRoute *route, cchar *header, cchar *value, int flags);

/**
    Set the route index document
    @description Set the name of the index document to serve. Index documents may be served when the request corresponds
        to a directory on the file system.
    @param route Route to modify
    @param path Path name to the index document. If the path is a relative path, it may be joined to the route 
        directory to create an absolute path.
    @return A reference to the route data. Otherwise return null if the route data for the given key was not found.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddRouteIndex(HttpRoute *route, cchar *path);

/**
    Add a route language suffix
    @description This configures the route pipeline by adding the given language for request processing.
        The language definition includes a suffix which will be added to the request filename.
    @param route Route to modify
    @param language Language symbolic name. For example: "en" for english.
    @param suffix Extension suffix to add when creating filenames for the request. For example: "fr" to add to "index.html"
        could produce: "index.fr.html".
    @param flags Set to HTTP_LANG_BEFORE to insert the suffix before the filename extension. Set to HTTP_LANG_AFTER to 
        append after the extension. For example: HTTP_LANG_AFTER would produce "index.html.fr".
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpAddRouteLanguageSuffix(HttpRoute *route, cchar *language, cchar *suffix, int flags);

/**
    Add a route language directory
    @description This configures the route pipeline by adding the given language content directory.
        When creating filenames for matching requests, the language directory is prepended to the request filename.
    @param route Route to modify
    @param language Language symbolic name. For example: "en" for english.
    @param path File system directory to contain content for matching requests.
    @return Zero if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpAddRouteLanguageDir(HttpRoute *route, cchar *language, cchar *path);

/**
    Add a route param check
    @description This configures the route to match a request only if the specified param field matches a specific value.
    @param route Route to modify
    @param field Param field to interrogate
    @param value Header value that will match
    @param flags Set to HTTP_ROUTE_NOT to negate the query test
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddRouteParam(HttpRoute *route, cchar *field, cchar *value, int flags);

/**
    Add a route update rule
    @description This configures the route pipeline by adding processing update rules for a request.
        Updates are built-in rules that can be applied to routes.
    @param route Route to modify
    @param name Update rule to add. Supported update rules include: "cmd", "field" and "lang". 
        \n\n
        The "cmd" rule is used to run external commands. For example: "cmd touch /tmp/filename".
        \n\n
        The "param" rule is used to set values in the request param fields. For example: "param priority high". 
        \n\n
        The "lang" update rule is used internally to implement the various language options.
        See #httpSetRouteTarget for a list of the token values that can be included in the condition rule details.
    @param details Update rule parameters.
    @param flags Reserved.
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpAddRouteUpdate(HttpRoute *route, cchar *name, cchar *details, int flags);

/**
    Add a route for static content under a "static" directory. This can be used for ESP applications. Use the EspDir
        appweb configuration file directive to modify the "static" directory.
    @description This will add a route for static content. This will add the following route:
    <table>
        <tr><td>Name</td><td>Method</td><td>Pattern</td><td>Target</td></tr>
        <tr><td>static</td><td>GET</td><td>^/static(/(.)*$</td><td>$1</td></tr>
    </table>
    @param parent Parent route from which to inherit configuration.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpAddStaticRoute(HttpRoute *parent);

/**
    Backup the route log if required
    @description If the log file is greater than the maximum configured, or MPR_ANEW was set via httpSetRouteLog, then 
    archive the log.
    @param route Route to modify
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpBackupRouteLog(HttpRoute *route);

/**
    Clear the pipeline stages for the route
    @description This resets the configured pipeline stages for the route.
    @param route Route to modify
    @param direction Set to HTTP_STAGE_TX for transmit direction and HTTP_STAGE_RX for receive data flow.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpClearRouteStages(HttpRoute *route, int direction);

/**
    Create a route suitable for use as an alias
    @description The parent supplies the owning host for the route. A route is not added to its owning host until it
        is finalized by calling #httpFinalizeRoute
    @param parent Parent route to inherit from
    @param pattern Pattern to match URIs 
    @param path File system directory containing documents for this route
    @param status Http redirect status for matching requests. Set to zero if not using redirects
    @return Allocated HttpRoute object
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC HttpRoute *httpCreateAliasRoute(HttpRoute *parent, cchar *pattern, cchar *path, int status);

/**
    Create a configured route 
    @description This creates a route and configures the request pipeline with range, chunk and upload filters.
    @param host HttpHost object owning the route
    @param serverSide Set to "true" if this is a server side route. Set to "false" for client side.
    @return Allocated HttpRoute object
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC HttpRoute *httpCreateConfiguredRoute(struct HttpHost *host, int serverSide);

/**
    Create a default route for a host
    @description When the route is fully configured, it should be finalized which will add it to its owning host.
    @param host HttpHost object owning the route
    @return Allocated HttpRoute object
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC HttpRoute *httpCreateDefaultRoute(struct HttpHost *host);

/**
    Create a route inherited from a parent route
    @description When the route is fully configured, it should be finalized which will add it to its owning host.
    @param route Parent route from which to inherit
    @return Allocated HttpRoute object
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC HttpRoute *httpCreateInheritedRoute(HttpRoute *route);

/**
    Create a route for use with the Action Handler
    @description This call creates a route inheriting from a parent route. The new route is configured for use with the
        actionHandler and the given callback procedure.
    @param parent Parent route from which to inherit
    @param pattern Pattern to match URIs 
    @param action Action to invoke
    @return Newly created route
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC HttpRoute *httpCreateActionRoute(HttpRoute *parent, cchar *pattern, HttpAction action);

/**
    Create a route for a host
    @description This call creates a bare route without inheriting from a parent route.
    When the route is fully configured, it should be finalized which will add it to its owning host.
    @param host HttpHost object owning the route
    @return Allocated HttpRoute object
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC HttpRoute *httpCreateRoute(struct HttpHost *host);

/**
    Define a route
    @description This creates a route and then configures it using the given parameters. The route is finalized and
        added to the parent host.
    @param parent Parent route from which to inherit configuration.
    @param name Route name to define.
    @param methods Http methods for which this route is active
    @param pattern Matching URI pattern for which this route will qualify
    @param target Route target string expression. This is used by handlers to determine the physical or virtual resource
        to serve.
    @param source Source file pattern containing the resource to activate or serve. 
    @return Created route.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC HttpRoute *httpDefineRoute(HttpRoute *parent, cchar *name, cchar *methods, cchar *pattern, 
    cchar *target, cchar *source);

/**
    Define a route condition rule
    @description This creates a new condition rule.
    @param name Condition name 
    @param proc Condition function to process the condition during route matching.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpDefineRouteCondition(cchar *name, HttpRouteProc *proc);

/**
    Define a route target rule
    @description This creates a new target rule.
    @param name Target name 
    @param proc Target function to process the target during route matching.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpDefineRouteTarget(cchar *name, HttpRouteProc *proc);

/**
    Define a route update rule
    @description This creates a new update rule.
    @param name Update name 
    @param proc Update function to process the update during route matching.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpDefineRouteUpdate(cchar *name, HttpRouteProc *proc);

/**
    Enable use of the TRACE Http method
    @param route Route to modify
    @param on Set to true to enable the trace method
    @ingroup HttpLimits
    @stability Evolving
 */
PUBLIC void httpEnableTraceMethod(HttpRoute *route, bool on);

/**
    Enable use of the DELETE and PUT methods
    @param route Route to modify
    @param on Set to true to enable the DELETE and PUT Http methods
    @ingroup HttpLimits
    @stability Evolving
 */
PUBLIC void httpEnablePutMethod(HttpRoute *route, bool on);

/**
    Expand route variables in a string
    @param route Route to modify
    @param str String to expand
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC cchar *httpExpandRouteVars(HttpRoute *route, cchar *str);

/**
    Finalize a route
    @description A route must be finalized to add it to its owning hosts list of routes.
    @param route Route to modify
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpFinalizeRoute(HttpRoute *route);

/**
    Get extra route data
    @description Routes can store extra configuration information indexed by key. This is used by handlers, filters,
        connectors and updates to store additional information on a per-route basis.
    @param route Route to modify
    @param key Unique string key to identify the data.
    @return A reference to the route data. Otherwise return null if the route data for the given key was not found.
    @see httpGetRouteData
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void *httpGetRouteData(HttpRoute *route, cchar *key);

/**
    Get the route directory
    @description Routes can define a default directory for documents to serve. This value may be used by
        target rules to calculate the response filename.
    @param route Route to modify
    @return The route documents directory pathname.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC cchar *httpGetRouteDir(HttpRoute *route);

/**
    Get the route method list
    @param route Route to examine
    @return The the list of support methods. Return NULL if not method list is defined.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC cchar *httpGetRouteMethods(HttpRoute *route);

/**
    Get a path token variable
    @param route Route to get
    @param key Token key value
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC cchar *httpGetRouteVar(HttpRoute *route, cchar *key);

/**
    Graduate the limits from the parent route.
    @description This creates a unique limit structure for the route if it is currently inheriting its parents limits.
    @param route Route to modify
    @param limits Limits to use if graduating.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC HttpLimits *httpGraduateLimits(HttpRoute *route, HttpLimits *limits);

/** 
    Create a URI link. 
    @description Create a URI link by expansions tokens based on the current request and route state.
    The target parameter may contain partial or complete URI information. The missing parts 
    are supplied using the current request and route tables. The resulting URI is a normalized, server-local 
    URI (that begins with "/"). The URI will include any defined route prefix, but will not include scheme, host or 
    port components.
    @param [in] conn HttpConn connection object 
    @param target The URI target. The target parameter can be a URI string or JSON style set of options. 
        The target will have any embedded "{tokens}" expanded by using token values from the request parameters.
        If the target has an absolute URI path, that path is used directly after tokenization. If the target begins with
        "~", that character will be replaced with the route prefix. This is a very convenient way to create application 
        top-level relative links.
        \n\n
        If the target is a string that begins with "{AT}" it will be interpreted as a controller/action pair of the 
        form "{AT}Controller/action". If the "controller/" portion is absent, the current controller is used. If 
        the action component is missing, the "list" action is used. A bare "{AT}" refers to the "list" action 
        of the current controller.
        \n\n
        If the target starts with "{" it is interpreted as being a JSON style set of options that describe the link.
        If the target is a relative URI path, it is appended to the current request URI path.  
        \n\n
        If the is a JSON style of options, it can specify the URI components: scheme, host, port, path, reference and
        query. If these component properties are supplied, these will be combined to create a URI.
        \n\n
        If the target specifies either a controller/action or a JSON set of options, The URI will be created according 
        to the route URI template. The template may be explicitly specified
        via a "route" target property. Otherwise, if an "action" property is specified, the route of the same
        name will be used. If these don't result in a usable route, the "default" route will be used. 
        \n\n
        These are the properties supported in a JSON style "{ ... }" target:
        <ul>
            <li>scheme String URI scheme portion</li>
            <li>host String URI host portion</li>
            <li>port Number URI port number</li>
            <li>path String URI path portion</li>
            <li>reference String URI path reference. Does not include "#"</li>
            <li>query String URI query parameters. Does not include "?"</li>
            <li>controller String Controller name if using a Controller-based route. This can also be specified via
                the action option.</li>
            <li>action String Action to invoke. This can be a URI string or a Controller action of the form
                {AT}Controller/action.</li>
            <li>route String Route name to use for the URI template</li>
        </ul>
    @param options Hash of option values for embedded tokens.
    @return A normalized, server-local Uri string.
    @ingroup HttpRoute
    @stability Evolving
    @remarks Examples:<pre>
    httpLink(conn, "http://example.com/index.html", 0);
    httpLink(conn, "/path/to/index.html", 0);
    httpLink(conn, "../images/splash.png", 0);
    httpLink(conn, "~/static/images/splash.png", 0);
    httpLink(conn, "${app}/static/images/splash.png", 0);
    httpLink(conn, "@controller/checkout", 0);
    httpLink(conn, "@controller/")                //  Controller = Controller, action = index
    httpLink(conn, "@init")                       //  Current controller, action = init
    httpLink(conn, "@")                           //  Current controller, action = index
    httpLink(conn, "{ action: '@post/create' }", 0);
    httpLink(conn, "{ action: 'checkout' }", 0);
    httpLink(conn, "{ action: 'logout', controller: 'admin' }", 0);
    httpLink(conn, "{ action: 'admin/logout'", 0);
    httpLink(conn, "{ product: 'candy', quantity: '10', template: '/cart/${product}/${quantity}' }", 0);
    httpLink(conn, "{ route: '~/STAR/edit', action: 'checkout', id: '99' }", 0);
    httpLink(conn, "{ template: '~/static/images/${theme}/background.jpg', theme: 'blue' }", 0);
</pre>
*/
PUBLIC char *httpLink(HttpConn *conn, cchar *target, MprHash *options);

/**
    Lookup an error document by HTTP status code
    @description This looks up error documents configured via #httpAddRouteErrorDocument
    @param route Route to modify
    @param status HTTP status code integer 
    @return URI associated with the error document for the requested status.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC cchar *httpLookupRouteErrorDocument(HttpRoute *route, int status);

/**
    Make a filename path
    @description This makes a filename by expanding the tokens "${token}" and then normalizing the path. Relative paths
        are resolved relative to the optional dir parameter.
        The supported tokens are:
        <ul>  
            <li>DOCUMENTS_DIR - for the default directory containing documents to serve</li>
            <li>HOME_DIR - for the directory containing the web server configuration files</li>
            <li>BIN_DIR - for the shared library directory. E.g. /usr/lib/appweb/bin </li>
            <li>OS - for the operating system name. E.g. LINUX, MACOSX, VXWORKS, or WIN</li>
            <li>PRODUCT - for the product name</li>
            <li>VERSION - for the product version. E.g. 4.0.2</li>
        </ul>  
        Additional tokens can be defined via #httpSetRouteVar.
    @param route Route to modify
    @param dir Directory to use as a base directory for relative paths.
    @param path Path name to examine
    @return A resolved absolute path name.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC char *httpMakePath(HttpRoute *route, cchar *dir, cchar *path);

/**
    Map the request URI and route target to a filename
    @description This sets the HttpTx filename, ext, etag and info fields.
    @param conn HttpConn connection object 
    @param route Route to modify
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpMapFile(HttpConn *conn, HttpRoute *route);

/**
    Match a route against the current request 
    @description This tests if a route matches the current request on a connection. This call is automatically called
        by #httpRouteRequest for incoming requests to a server.
    @param conn HttpConn connection object 
    @param route Route to modify
    @return HTTP_ROUTE_OK if the request matches. Return HTTP_ROUTE_REMATCH if the request has been modified and
        route matching must be restarted.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpMatchRoute(HttpConn *conn, HttpRoute *route);

/**
    Reset the route pipeline
    @description This completely resets the pipeline and discards inherited pipeline configuration. This resets the
        error documents, expiry cache values, extensions, handlers, input and output stage configuration.
    @param route Route to modify
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpResetRoutePipeline(HttpRoute *route);

/**
    Set the route authentication
    @description This defines the authentication configuration for basic and digest authentication for the route.
    @param route Route to modify
    @param auth Authentication object
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteAuth(HttpRoute *route, HttpAuth *auth);

/**
    Control file upload auto delete
    @description This controls whether files are auto-deleted after the handler runs to service a request.
    @param route Route to modify
    @param on Set to true to enable auto-delete. Auto-delete is enabled by default.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteAutoDelete(HttpRoute *route, bool on);

/**
    Contol content compression for the route
    @description This configures content compression for the route. Some handlers observe the content compression status
        and will attempt to use or compress content before transmitting to the client. The fileHandler is currently
        the only handler that uses this capability.
    @param route Route to modify
    @param flags Set to HTTP_ROUTE_GZIP to enable the fileHandler to serve eqivalent compressed content with a "gz"
        extension.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteCompression(HttpRoute *route, int flags);

/**
    Set the connector to use for a route
    @param route Route to modify
    @param name Connector name to use for this route
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpSetRouteConnector(HttpRoute *route, cchar *name);

/**
    Set route data
    @description Routes can store extra configuration information indexed by key. This is used by handlers, filters,
        connectors and updates to store additional information on a per-route basis.
    @param route Route to modify
    @param key Unique string to identify the data
    @param data Data object. This must be allocated via mprAlloc.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteData(HttpRoute *route, cchar *key, void *data);

/**
    Set the default language for the route
    @description This call defines the default language to serve if the client does not provide an Accept HTTP header
        with language preference instructions.
    @param route Route to modify
    @param language Language symbolic name. For example: "en" for english.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteDefaultLanguage(HttpRoute *route, cchar *language);

/**
    Set the route directory
    @description Routes can define a default directory for documents to serve. This value may be used by
        target rules to calculate the response filename.
    @param route Route to modify
    @param path Directory path name for the route content
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteDir(HttpRoute *route, cchar *path);

/**
    Update the route flags
    @description Low level routine to manipulate the route flags
    @param route Route to modify
    @param flags Flags mask 
    @ingroup HttpRoute
    @stability Evolving
    @internal
 */
PUBLIC void httpSetRouteFlags(HttpRoute *route, int flags);

/**
    Set the handler to use for a route
    @description This defines the stage handler to use in the request pipline for requests matching this route.
        Note that you can also use httpAddRouteHandler which configures a set of handlers that will match by extension.
    @param route Route to modify
    @param name Handler name to define
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpSetRouteHandler(HttpRoute *route, cchar *name);

/**
    Set the route directory for configuration files
    @description Routes can define a default directory for configuration files. 
    @param route Route to modify
    @param home Directory path name for configuration files 
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteHome(HttpRoute *route, cchar *home);

/*
    Define the owning host for a route.
    @description WARNING: this should not be called by users.
    @param route Route to modify
    @param host HttpHost object
    @ingroup HttpRoute
    @stability Internal
    @internal
 */
PUBLIC void httpSetRouteHost(HttpRoute *route, struct HttpHost *host);

/**
    Define the methods for the route
    @description This defines the set of valid HTTP methods for requests to match this route
    @param route Route to modify
    @param methods Set to a comma or space separated list of methods. Can also set to "All" or "*" for all possible 
        methods.  Standard methods include: "DELETE, GET, OPTIONS, POST, PUT, TRACE".
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteMethods(HttpRoute *route, cchar *methods);

/**
    Set the route name
    @description Symbolic route names are used by httpLink and when displaying route tables.
    @param route Route to modify
    @param name Unique symbolic name for the route. If a name is not defined, the route pattern will be used as the name. 
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteName(HttpRoute *route, cchar *name);

/**
    Define a path token variable
    @description The #httpMakePath routine and route conditions, updates, headers, fields and targets will expand 
        tokenized expressions "${token}". Additional tokens can be defined via this API.
    @param route Route to modify
    @param token Name of the token to define 
    @param value Value of the token
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteVar(HttpRoute *route, cchar *token, cchar *value);

/**
    Set the route pattern
    @description This call defines the route regular expression pattern that is used to match against the request URI.
        The route pattern is an enhanced JavaScript-compatibile regular expression. It is enhanced by optionally 
        embedding braced tokens "{name}" in the patter. During request URI matching, these tokens are extracted and
        defined in the request params and are available to the request. The normal regular expression repeat syntax 
        also uses "{}". To use the traditional (uncommon) repeat syntax, back quote with "\\".
        Sub-expressions and token expressions are also available in various rules as numbered tokens "$1". For example:
        the pattern "/app/(.*)(\.html)$" will permit a file target "$1.${request.Language=fr}.$2".
    @param route Route to modify
    @param pattern Route regular expression pattern 
    @param flags Set to HTTP_ROUTE_NOT to negate the pattern match result
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRoutePattern(HttpRoute *route, cchar *pattern, int flags);

/**
    Set the route prefix
    @description Routes may have a prefix which will be stripped from the request URI if the request matches.
        The prefix is made available as the "${request:prefix}" token and also as the ScriptName via some handlers.
    @param route Route to modify
    @param prefix URI prefix to define for the route. 
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRoutePrefix(HttpRoute *route, cchar *prefix);

/**
    Set the script to service the route.
    @description This is used by handlers to add a per-route script for processing. Ejscript uses this to specify
        the server script. Either a literal script or a path to a script filename can be provided.
    @param route Route to modify
    @param script Literal script to execute.
    @param scriptPath Pathname to the script file to execute
    @ingroup HttpRoute
    @stability Evolving
    @internal
 */
PUBLIC void httpSetRouteScript(HttpRoute *route, cchar *script, cchar *scriptPath);

/**
    Set the source code module for the route
    @description Some handlers can dynamically load web applications and controllers to serve requests.
    @param route Route to modify
    @param source Source path or description 
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteSource(HttpRoute *route, cchar *source);

/**
    Set a route target
    @description This configures the route pipeline by defining a route target. The route target is interpreted by
        the selected route handler to process the request. 
        Route targets can contain symbolic tokens that are expanded at run-time with their corresponding values. There are 
        three classes of tokens:
        <ul>  
            <li>System and Route varibles - such as DOCUMENTS_DIR, HOME_DIR, BIN_DIR, PRODUCT, OS, VERSION.</li>
            <li>Route URI tokens - these are the braced tokens in the route pattern.</li>
            <li>Request fields - these are request state and property values.</li>
        </ul>
        System and URI tokens are of the form: "${token}" where "token" is the name of the variable or URI token. 
        Request fields are of the form: "${family:name=defaultValue}" where the family defines a set of values. 
        If the named field is not present, an optional default value "=defaultValue" will be used instead.
        These supported request field families are:
        <ul>  
            <li>header - for request HTTP header values</li>
            <li>param - for request params</li>
            <li>query - for request query field values</li>
            <li>request - for request details</li>
            <li>Any URI pattern  token</li>
        </ul>
        For example: "run ${header:User-Agent}" to select the client's browser string passed in the HTTP headers.
        For example: "run ${field:name}" to select the client's browser string passed in the HTTP headers.
        For example: "run ${name}.html" where {name} was a token in the route pattern.
        For example: "run ${name}.html" where {name} was a token in the route pattern.
        The supported request key names are:
        <ul>
            <li>clientAddress - The client IP address</li>
            <li>clientPort - The client port number</li>
            <li>error - Any request or connection error message</li>
            <li>ext - The request extension</li>
            <li>extraPath - The request extra path after the script extension</li>
            <li>filename - The mapped request filename in physical storage</li>
            <li>language - The selected language for the request</li>
            <li>languageDir - The langauge directory</li>
            <li>host - The host name owning the route for the request</li>
            <li>method - The request HTTP method</li>
            <li>originalUri - The original, pre-decoded URI</li>
            <li>pathInfo - The path portion of the URI after the host and port information</li>
            <li>prefix - The route prefix</li>
            <li>query - The request query information</li>
            <li>reference - The request reference fragment. This is the URI portion after "#"</li>
            <li>scheme - The request protocol scheme. E.g. "http"</li>
            <li>scriptName - The request script or application name</li>
            <li>serverAddress - The server IP address</li>
            <li>serverPort - The server port number</li>
            <li>uri - The full request URI. May be modified by routes, handlers and filters</li>
        </ul>
        Also see #httpMakePath for additional tokens (DOCUMENTS_DIR, HOME_DIR, BIN_DIR, PRODUCT, OS, VERSION).
    @param route Route to modify
    @param name Target rule to add. Supported update rules include:
        "close", "redirect", "run" and "write". 
        \n\n
        The "close" rule is used to do abortive closes for the request. This is useful for ward off known security attackers.
        For example: "close immediate". The "close" rule takes no addition parameters. 
        \n\n
        \n\n
        The "redirect" rule is used to redirect the request to a new resource. For example: "redirect 302 /tryAgain.html". 
        The "redirect" takes the form: "redirect status URI". The status code is used as the HTTP response
        code. The URI can be a fully qualified URI beginning with "http" or it can be a relative URI.
        \n\n
        The "run" target is used to run the configured handler to respond to the request.
        For example: "file ${DOCUMENTS}/${request.uri}.gz". 
        \n\n
        The "write" rule is used to write literal data back to the client. For example: "write 200 Hello World\r\n". 
        The "write" rule takes the form: "write [-r] status message". Write data is by default HTML encoded to help
        eliminate XSS security exposures. The "-r" option selects "raw" output and bypasses the HTML encoding of the
        write data string. 
        \n\n
        WARNING: Take great care when using raw writes with tokens. Write data is not HTML encoded and echoing back to
        raw data to the client can cause XSS and other security issues.
        The status field defines the HTTP status code to use in the response.
    @param details Update rule parameters.
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpSetRouteTarget(HttpRoute *route, cchar *name, cchar *details);

/**
    Set the route template
    @description Set the route URI template uses when constructing URIs via httpLink.
    @param route Route to modify
    @param tplate URI template to use. Templates may contain embedded tokens "{token}" where the token names correspond
        to the token names in the route pattern. 
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteTemplate(HttpRoute *route, cchar *tplate);

/**
    Set the route trace filter
    @description Trace filters include or exclude trace items based on the request filename extension.
    @param route HttpRoute object
    @param dir Trace direction. Set to HTTP_TRACE_TX or HTTP_TRACE_RX
    @param levels Trace class levels. Indicies are: HTTP_TRACE_CONN, HTTP_TRACE_FIRST, HTTP_TRACE_HEADER, HTTP_TRACE_BODY,
        HTTP_TRACE_LIMITS, HTTP_TRACE_TIME.
    @param len Maximum content length eligible for tracing.
    @param include Comma or space separated list of extensions to include in tracing
    @param exclude Comma or space separated list of extensions to exclude from tracing
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpSetRouteTraceFilter(HttpRoute *route, int dir, int levels[HTTP_TRACE_MAX_ITEM], 
        ssize len, cchar *include, cchar *exclude);

/**
    Define the maximum number of workers for a route
    @param route Route to modify
    @param workers Maximum number of workers for this route
    @ingroup HttpRoute
    @stability Evolving
    @internal
 */
PUBLIC void httpSetRouteWorkers(HttpRoute *route, int workers);

/**
    Expand a template string using given options
    @description This expands a string with embedded tokens of the form "${token}" using values from the given options.
    @param conn HttpConn connection object created via #httpCreateConn
    @param tplate Template string to process
    @param options Hash of option values for embedded tokens.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC char *httpTemplate(HttpConn *conn, cchar *tplate, MprHash *options);

/**
    Tokenize a string based on route data
    @description This is a utility routine to parse a string into tokens given a format specifier. 
    Mandatory tokens can be specified with "%" format specifier. Optional tokens are specified with "?" format. 
    Supported tokens:
    <ul>
    <li>%B - Boolean. Parses: on/off, true/false, yes/no.</li>
    <li>%N - Number. Parses numbers in base 10.</li>
    <li>%S - String. Removes quotes.</li>
    <li>%P - Path string. Removes quotes and expands ${PathVars}. Resolved relative to host->dir (Home).</li>
    <li>%W - Parse words into a list</li>
    <li>%! - Optional negate. Set value to HTTP_ROUTE_NOT present, otherwise zero.</li>
    </ul>
    Values wrapped in quotes will have the outermost quotes trimmed.
    @param route Route to modify
    @param str String to expand
    @param fmt Format string specifier
    @return True if the string can be successfully parsed.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC bool httpTokenize(HttpRoute *route, cchar *str, cchar *fmt, ...);

/**
    Tokenize a string based on route data
    @description This is a utility routine to parse a string into tokens given a format specifier. 
    This call is similar to #httpTokenize but uses a va_list argument.
    @param route Route to modify
    @param str String to expand
    @param fmt Format string specifier
    @param args Varargs argument list
    @return True if the string can be successfully parsed.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC bool httpTokenizev(HttpRoute *route, cchar *str, cchar *fmt, va_list args);

/**
    Configure the route access log
    @param route Route to modify
    @param path Path for route access log file.
    @param size Maximum size of the log file before archiving
    @param backup Set to true to create a backup of the log file if archiving.
    @param format Log file format
    @param flags Set to MPR_LOG_ANEW to archive the log when the application reboots.
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC int httpSetRouteLog(HttpRoute *route, cchar *path, ssize size, int backup, cchar *format, int flags);

/**
    Write data to the route access log
    @description Write data after archiving if required.
    @param route Route to modify
    @param buf Data buffer to write
    @param len Size of the data buffer.
    @ingroup HttpRoute
    @stability Evolving
 */
PUBLIC void httpWriteRouteLog(HttpRoute *route, cchar *buf, ssize len);

/*
    Internal
 */
PUBLIC void httpLogRequest(HttpConn *conn);
PUBLIC MprFile *httpOpenRouteLog(HttpRoute *route);
PUBLIC int httpStartRoute(HttpRoute *route);
PUBLIC void httpStopRoute(HttpRoute *route);
PUBLIC char *httpExpandUri(HttpConn *conn, cchar *str);

/*********************************** Session ***************************************/

#define HTTP_SESSION_COOKIE     "-http-session-"    /**< Session cookie name */
#define HTTP_SESSION_USERNAME   "_:USERNAME:_"      /**< Username variable */
#define HTTP_SESSION_AUTHVER    "_:VERSION:_"       /**< Auth version number */

/**
    Session state object
    @defgroup HttpSession HttpSession
    @see httpAllocSession httpCreateSession httpDestroySession httpGetSession httpGetSessionObj
        httpRemoveSessionVar httpGetSessionID httpSetSessionObj httpSetSessionVar
    @stability Internal
 */
typedef struct HttpSession {
    char            *id;                        /**< Session ID key */
    HttpConn        *conn;                      /**< Owning connection */
    MprCache        *cache;                     /**< Cache store reference */
    MprTicks        lifespan;                   /**< Session inactivity timeout (msecs) */
} HttpSession;

/**
    Allocate a new session state object.
    @param conn Http connection object
    @param id Unique session state ID
    @param lifespan Session lifespan in ticks
    @return A session state object
    @ingroup HttpSession
    @stability Internal
 */
PUBLIC HttpSession *httpAllocSession(HttpConn *conn, cchar *id, MprTicks lifespan);

/**
    Create a session object.
    @description This call creates a session object. If one already exists, it is destroyed and a fresh session
        is created. Use httpGetSession to retrieve an existing session object.
    @param conn Http connection object
    @return A session state object
    @ingroup HttpSession
    @stability Internal
 */
PUBLIC HttpSession *httpCreateSession(HttpConn *conn);

/**
    Destroy a session state object.
    This destroys a session. It will emit an expired cookie to force the client to remove the old session cookie.
    @description
    @param conn Http connection object.
    @ingroup HttpSession
    @stability Internal
 */
PUBLIC void httpDestroySession(HttpConn *conn);

/**
    Get a session state object.
    This will optionally create a session if one does not already exist. It will not re-create a session that exists.
    @description
    @param conn Http connection object
    @param create Set to "true" to create a session state object if one does not already exist for this client
    @return A session state object
    @ingroup HttpSession
    @stability Evolving
 */
PUBLIC HttpSession *httpGetSession(HttpConn *conn, int create);

/**
    Get an object from the session state store.
    @description Retrieve an object from the session state store by deserializing all properties.
    @param conn Http connection object
    @param key Session state key
    @ingroup HttpSession
    @stability Evolving
 */
PUBLIC MprHash *httpGetSessionObj(HttpConn *conn, cchar *key);

/**
    Get a session state variable.
    @param conn Http connection object
    @param name Variable name to get
    @param defaultValue If the variable does not exist, return the defaultValue.
    @return The variable value or defaultValue if it does not exist.
    @ingroup HttpSession
    @stability Evolving
 */
PUBLIC cchar *httpGetSessionVar(HttpConn *conn, cchar *name, cchar *defaultValue);

/**
    Remove a session state variable
    @param conn Http connection object
    @param name Variable name to remove
    @return Zero if successful, otherwise a negative MPR error code.
    @ingroup HttpSession
    @stability Evolving
 */
PUBLIC int httpRemoveSessionVar(HttpConn *conn, cchar *name);

/**
    Set a session variable.
    @description
    @param conn Http connection object
    @param name Variable name to set
    @param value Variable value to use
    @return A session state object
    @ingroup HttpSession
    @stability Evolving
 */
PUBLIC int httpSetSessionVar(HttpConn *conn, cchar *name, cchar *value);

/**
    Get the session ID.
    @description
    @param conn Http connection object
    @return The session ID string
    @ingroup HttpSession
    @stability Evolving
 */
PUBLIC char *httpGetSessionID(HttpConn *conn);

/**
    Set an object into the session state store.
    @description Store an object in the session state store by serializing all properties.
    @param conn Http connection object
    @param key Session state key
    @param value Object to serialize
    @ingroup HttpSession
    @stability Evolving
 */
PUBLIC int httpSetSessionObj(HttpConn *conn, cchar *key, MprHash *value);

/********************************** HttpUploadFile *********************************/
/**
    Upload File
    @description Each uploaded file has an HttpUploadedFile entry. This is managed by the upload handler.
    @stability Evolving
    @defgroup HttpUploadFile HttpUploadFile
    @see httpAddUploadFile httpRemoveAllUploadedFiles httpRemoveUploadFile
    @stability Internal
 */
typedef struct HttpUploadFile {
    cchar           *filename;              /**< Local (temp) name of the file */
    cchar           *clientFilename;        /**< Client side name of the file */
    cchar           *contentType;           /**< Content type */
    ssize           size;                   /**< Uploaded file size */
} HttpUploadFile;

/**
    Add an Uploaded file
    @description Add an uploaded file to the Rx.files collection.
    @param conn HttpConn connection object created via #httpCreateConn
    @param id Unique identifier for the file  
    @param file Instance of HttpUploadFile
    @ingroup HttpUploadFile
    @stability Internal
    @internal
 */
PUBLIC void httpAddUploadFile(HttpConn *conn, cchar *id, HttpUploadFile *file);

/**
    Remove all uploaded files
    @description Remove all uploaded files from the temporary file store
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpUploadFile
    @stability Internal
    @internal
 */
PUBLIC void httpRemoveAllUploadedFiles(HttpConn *conn);

/**
    Remove an uploaded file
    @description Remove an uploaded file from the temporary file store
    @param conn HttpConn connection object created via #httpCreateConn
    @param id Identifier used with #httpAddUploadFile for the file
    @ingroup HttpUploadFile
    @stability Internal
    @internal
 */
PUBLIC void httpRemoveUploadFile(HttpConn *conn, cchar *id);

/********************************** HttpRx *********************************/
/* 
    Rx flags
    MOB - inconsistent with HTTP_TX_
 */
#define HTTP_DELETE             0x1         /**< DELETE method  */
#define HTTP_GET                0x2         /**< GET method  */
#define HTTP_HEAD               0x4         /**< HEAD method  */
#define HTTP_OPTIONS            0x8         /**< OPTIONS method  */
#define HTTP_POST               0x10        /**< Post method */
#define HTTP_PUT                0x20        /**< PUT method  */
#define HTTP_TRACE              0x40        /**< TRACE method  */
#define HTTP_CREATE_ENV         0x80        /**< Must create env for this request */
#define HTTP_IF_MODIFIED        0x100       /**< If-[un]modified-since supplied */
#define HTTP_CHUNKED            0x200       /**< Content is chunk encoded */
#define HTTP_ADDED_QUERY_PARAMS 0x400       /**< Query added to params */
#define HTTP_ADDED_FORM_PARAMS  0x800       /**< Form body data added to params */
#define HTTP_LIMITS_OPENED      0x1000      /**< Request limits opened */
#define HTTP_EXPECT_CONTINUE    0x2000      /**< Client expects an HTTP 100 Continue response */
#define HTTP_AUTH_CHECKED       0x4000      /**< User authentication has been checked */

/*  
    Incoming chunk encoding states
 */
#define HTTP_CHUNK_UNCHUNKED  0             /**< Data is not transfer-chunk encoded */
#define HTTP_CHUNK_START      1             /**< Start of a new chunk */
#define HTTP_CHUNK_DATA       2             /**< Start of chunk data */
#define HTTP_CHUNK_EOF        3             /**< End of last chunk */

/** 
    Http Rx
    @description Most of the APIs in the rx group still take a HttpConn object as their first parameter. This is
        to make the API easier to remember - APIs take a connection object rather than a rx or tx object.
    @defgroup HttpRx HttpRx
    @see HttpConn HttpRx HttpTx httpAddBodyVars httpAddParamsFromBuf httpAddParamsFromQueue httpContentNotModified 
        httpCreateCGIParams httpGetContentLength httpGetCookies httpGetParam httpGetParams httpGetHeader 
        httpGetHeaderHash httpGetHeaders httpGetIntParam httpGetLanguage httpGetQueryString httpGetReadCount httpGetStatus 
        httpGetStatusMessage httpMatchParam httpRead httpReadString httpSetParam httpSetIntParam httpSetUri 
        httpTestParam httpTrimExtraPath 
    @stability Internal
 */
typedef struct HttpRx {
    /* Ordered for debugging */
    char            *method;                /**< Request method */
    char            *uri;                   /**< Current URI (not decoded, may be rewritten) */
    char            *pathInfo;              /**< Path information after the scriptName (Decoded and normalized) */
    char            *scriptName;            /**< ScriptName portion of the uri (Decoded). May be empty or start with "/" */
    char            *extraPath;             /**< Extra path information (CGI|PHP) */
    int             eof;                    /**< All read data has been received (eof) */
    MprOff          bytesRead;              /**< Length of content read by user */
    MprOff          length;                 /**< Content length header value (ENV: CONTENT_LENGTH) */
    MprOff          remainingContent;       /**< Remaining content data to read (in next chunk if chunked) */

    HttpConn        *conn;                  /**< Connection object */
    HttpRoute       *route;                 /**< Route for request */
    HttpSession     *session;               /**< Session for request */
    int             sessionProbed;          /**< Session has been resolved */

    MprList         *etags;                 /**< Document etag to uniquely identify the document version */
    HttpPacket      *headerPacket;          /**< HTTP headers */
    MprHash         *headers;               /**< Header variables */
    MprList         *inputPipeline;         /**< Input processing */
    HttpUri         *parsedUri;             /**< Parsed request uri */
    MprHash         *requestData;           /**< General request data storage. Users must create hash table if required */
    MprTime         since;                  /**< If-Modified date */

    int             authenticated;          /**< Request has been authenticated */
    int             chunkState;             /**< Chunk encoding state */
    int             flags;                  /**< Rx modifiers */
    int             form;                   /**< Using mime-type application/x-www-form-urlencoded */
    int             needInputPipeline;      /**< Input pipeline required to process received data */
    int             ownParams;              /**< Do own parameter handling */
    int             streaming;              /**< Stream incoming content. Forms typically buffer and dont stream */
    int             traceLevel;             /**< General trace level for header level info */
    int             upload;                 /**< Request is using file upload */

    bool            ifModified;             /**< If-Modified processing requested */
    bool            ifMatch;                /**< If-Match processing requested */

    /*  
        Incoming response line if a client request 
     */
    int             status;                 /**< HTTP response status */
    char            *statusMessage;         /**< HTTP Response status message */

    /* 
        Header values
        MOB OPT - eliminate some of these
     */
    char            *accept;                /**< Accept header */
    char            *acceptCharset;         /**< Accept-Charset header */
    char            *acceptEncoding;        /**< Accept-Encoding header */
    char            *acceptLanguage;        /**< Accept-Language header */
    char            *authDetails;           /**< Header details: authorization|www-authenticate provided by peer */
    char            *cookie;                /**< Cookie header */
    char            *connection;            /**< Connection header */
    char            *contentLength;         /**< Content length string value */
    char            *hostHeader;            /**< Client supplied host name header */

    char            *pragma;                /**< Pragma header */
    char            *mimeType;              /**< Mime type of the request payload (ENV: CONTENT_TYPE) */
    char            *originalMethod;        /**< Original method from the client */
    char            *origin;                /**< Origin header (not used) */
    char            *originalUri;           /**< Original URI passed by the client */
    char            *redirect;              /**< Redirect route header */
    char            *referrer;              /**< Refering URL */
    char            *securityToken;         /**< Security form token */
    char            *upgrade;               /**< Protocol upgrade header */
    char            *userAgent;             /**< User-Agent header */

    HttpLang        *lang;                  /**< Selected language */
    MprHash         *params;                /**< Request params (Query and post data variables) */
    MprHash         *svars;                 /**< Server variables */
    HttpRange       *inputRange;            /**< Specified range for rx (post) data */
    char            *passDigest;            /**< User password digest for authentication */
    char            *paramString;           /**< Cached param data as a string */

    /*  
        Upload details
        MOB - move to an upload structure
     */
    MprHash         *files;                 /**< Uploaded files. Managed by the upload filter */
    char            *uploadDir;             /**< Upload directory */
    int             autoDelete;             /**< Automatically delete uploaded files */

    struct HttpWebSocket *webSocket;        /**< WebSocket state */

    /*
        Routing info
     */
    char            *target;                /**< Route target */
    int             matches[BIT_MAX_ROUTE_MATCHES * 2];
    int             matchCount;
} HttpRx;

/**
    Add query and form body data to params
    @description This adds query data and posted body data to the request params
    @param conn HttpConn connection object
    @ingroup HttpRx
    @stability Internal
    @internal
 */
PUBLIC void httpAddParams(HttpConn *conn);

/**
    Test if the content has not been modified
    @description This call tests if the file content to be served has been modified since the client last
        requested this resource. The client must provide an Etag and Since or If-Modified headers.
    @param conn HttpConn connection object
    @return True if the content is current and has not been modified.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC bool httpContentNotModified(HttpConn *conn);

/**
    Create CGI parameters
    @description This call creates request params corresponding to the standard CGI/1.1 environment variables. 
    This is used by the CGI and PHP handlers. It may also be useful to handlers that wish to expose CGI style 
    environment variables
    through the form vars interface.
    @param conn HttpConn connection object
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC void httpCreateCGIParams(HttpConn *conn);

/** 
    Get the receive body content length
    @description Get the length of the receive body content (if any). This is used in servers to get the length of posted
        data and in clients to get the response body length.
    @param conn HttpConn connection object created via #httpCreateConn
    @return A count of the response content data in bytes.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC MprOff httpGetContentLength(HttpConn *conn);

/** 
    Get the request cookies
    @description Get the cookies defined in the current requeset
    @param conn HttpConn connection object created via #httpCreateConn
    @return Return a string containing the cookies sent in the Http header of the last request
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC cchar *httpGetCookies(HttpConn *conn);

/**
    Get a request param
    @description Get the value of a named request param. Form variables are define via www-urlencoded query or post
        data contained in the request.
    @param conn HttpConn connection object
    @param var Name of the request param to retrieve
    @param defaultValue Default value to return if the variable is not defined. Can be null.
    @return String containing the request param's value. Caller should not free.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC cchar *httpGetParam(HttpConn *conn, cchar *var, cchar *defaultValue);

/**
    Get the request params table
    @description This call gets the form var table for the current request.
        Query data and www-url encoded form data is entered into the table after decoding.
        Use #mprLookupKey to retrieve data from the table.
    @param conn HttpConn connection object
    @return #MprHash instance containing the form vars
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC MprHash *httpGetParams(HttpConn *conn);

/**
    Get the request params table as a string
    @description This call gets the request params encoded as a string. The params are always in the same order 
        regardless of the form parameter order. Request parameters include query parameters, form data and routing 
        parameters.
    @param conn HttpConn connection object
    @return A string representation in www-urlencoded format.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC char *httpGetParamsString(HttpConn *conn);

/** 
    Get an rx http header.
    @description Get a http response header for a given header key.
    @param conn HttpConn connection object created via #httpCreateConn
    @param key Name of the header to retrieve. This should be a lower case header name. For example: "Connection"
    @return Value associated with the header key or null if the key did not exist in the response.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC cchar *httpGetHeader(HttpConn *conn, cchar *key);

/** 
    Get the hash table of rx Http headers
    @description Get the internal hash table of rx headers
    @param conn HttpConn connection object created via #httpCreateConn
    @return Hash table. See MprHash for how to access the hash table.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC MprHash *httpGetHeaderHash(HttpConn *conn);

/** 
    Get all the request http headers.
    @description Get all the rx headers. The returned string formats all the headers in the form:
        key: value\\nkey2: value2\\n...
    @param conn HttpConn connection object created via #httpCreateConn
    @return String containing all the headers. The caller must free this returned string.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC char *httpGetHeaders(HttpConn *conn);

/**
    Get a header string from the given hash.
    @description This returns a set of "key: value" lines in HTTP header format.
    @param hash Hash table to examine
    @ingroup HttpRx
    @stability Internal
    @internal
 */
PUBLIC char *httpGetHeadersFromHash(MprHash *hash);

/**
    Get a form variable as an integer
    @description Get the value of a named form variable as an integer. Form variables are define via 
        www-urlencoded query or post data contained in the request.
    @param conn HttpConn connection object
    @param var Name of the form variable to retrieve
    @param defaultValue Default value to return if the variable is not defined. Can be null.
    @return Integer containing the form variable's value
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC int httpGetIntParam(HttpConn *conn, cchar *var, int defaultValue);

/**
    Get the language to use for the request
    @description This call tests if the file content to be served has been modified since the client last
        requested this resource. The client must provide an Etag and Since or If-Modified headers.
    @param conn HttpConn connection object
    @param spoken Hash table of HttpLang records. This is typically route->languages. 
    @param defaultLang Default language to use if none specified in the request Accept-Language header.
    @return A HttpLang reference, or null if no language requested or no language found in the spoken table.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC HttpLang *httpGetLanguage(HttpConn *conn, MprHash *spoken, cchar *defaultLang);

/**
    Get a path extension 
    @param path File pathname to examine
    @return The path extension sans "."
    @ingroup HttpRx
    @stability Stable
  */
PUBLIC char *httpGetPathExt(cchar *path);

/** 
    Get the request query string
    @description Get query string sent with the current request.
    @param conn HttpConn connection object
    @return String containing the request query string. Caller should not free.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC cchar *httpGetQueryString(HttpConn *conn);

/**
    Get the number of bytes that can be read from the read queue
    @param conn HttpConn connection object
    @return The number of bytes available in the read queue for the connection
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC ssize httpGetReadCount(HttpConn *conn);

/** 
    Get the response status 
    @param conn HttpConn connection object created via #httpCreateConn
    @return An integer Http response code. Typically 200 is success.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC int httpGetStatus(HttpConn *conn);

/** 
    Get the Http response status message. The Http status message is supplied on the first line of the Http response.
    @param conn HttpConn connection object created via #httpCreateConn
    @returns A Http status message. 
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC char *httpGetStatusMessage(HttpConn *conn);

/**
    Match a form variable with an expected value
    @description Compare a form variable and return true if it exists and its value matches.
    @param conn HttpConn connection object
    @param var Name of the form variable 
    @param expected Expected value to match with
    @return True if the value matches
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC bool httpMatchParam(HttpConn *conn, cchar *var, cchar *expected);

/** 
    Read rx body data. This will read available body data. If in sync mode, this call may block. If in async
    mode, the call will not block and will return with whatever data is available.
    @param conn HttpConn connection object created via #httpCreateConn
    @param buffer Buffer to receive read data
    @param size Size of buffer. 
    @return The number of bytes read
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC ssize httpRead(HttpConn *conn, char *buffer, ssize size);

/** 
    Read response data as a string. This will read all rx body and return a string that the caller should free. 
    This will block and should not be used in async mode.
    @param conn HttpConn connection object created via #httpCreateConn
    @returns A string containing the rx body. Caller should free.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC char *httpReadString(HttpConn *conn);

/**
    Set a request param value
    @description Set the value of a named request param to a string value. Form variables are define via 
        www-urlencoded query or post data contained in the request.
    @param conn HttpConn connection object
    @param var Name of the request param to retrieve
    @param value Default value to return if the variable is not defined. Can be null.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC void httpSetParam(HttpConn *conn, cchar *var, cchar *value);

/**
    Set an integer request param value
    @description Set the value of a named request param to an integer value. Form variables are define via 
        www-urlencoded query or post data contained in the request.
    @param conn HttpConn connection object
    @param var Name of the request param to retrieve
    @param value Default value to return if the variable is not defined. Can be null.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC void httpSetIntParam(HttpConn *conn, cchar *var, int value);

/**
    Set a new HTTP method for processing
    @description This modifies the request method to alter request processing. The original method is preserved in
        the HttpRx.originalMethod field. This is only useful to do before request routing has matched a route.
    @param conn HttpConn connection object
    @param method New method to use. 
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC void httpSetMethod(HttpConn *conn, cchar *method);

/**
    Set a new URI for processing
    @description This modifies the request URI to alter request processing. The original URI is preserved in
        the HttpRx.originalUri field. This is only useful to do before request routing has matched a route.
    @param conn HttpConn connection object
    @param uri New URI to use. The URI can be fully qualified starting with a scheme ("http") or it can be 
        a partial/relative URI. Missing portions of the URI will be completed with equivalent portions from the
        current URI. For example: if the current request URI was http://example.com:7777/index.html, then
        a call to httpSetUri(conn, "/new.html", 0)  will set the request URI to http://example.com:7777/new.html.
        The request script name will be reset and the pathInfo will be set to the path portion of the URI.
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC int httpSetUri(HttpConn *conn, cchar *uri);

/**
    Test if a request param is defined
    @param conn HttpConn connection object
    @param var Name of the request param to retrieve
    @return True if the request param is defined
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC int httpTestParam(HttpConn *conn, cchar *var);

/**
    Trim extra path from the URI
    @description This call trims extra path information after the uri extension. This is used by CGI and PHP. 
    The strategy is to heuristically find the script name in the uri. This is assumed to be the original uri 
    up to and including first path component containing a "." Any path information after that is regarded as 
    extra path.  WARNING: Extra path is an old, unreliable, CGI specific technique. Do not use directories 
    with embedded periods.
    @param conn HttpConn connection object
    @ingroup HttpRx
    @stability Stable
 */
PUBLIC void httpTrimExtraPath(HttpConn *conn);

/**
    Pump the Http engine for a request
    @param conn HttpConn connection object
    @param packet Optional packet of input data. Set to NULL if calling from user handlers.
    @return True if the request is completed successfully.
    @ingroup HttpRx
    @stability Internal
 */
PUBLIC bool httpPumpRequest(HttpConn *conn, HttpPacket *packet);

/* Internal */
PUBLIC void httpCloseRx(struct HttpConn *conn);
PUBLIC HttpRange *httpCreateRange(HttpConn *conn, MprOff start, MprOff end);
PUBLIC HttpRx *httpCreateRx(HttpConn *conn);
PUBLIC void httpDestroyRx(HttpRx *rx);
PUBLIC bool httpMatchEtag(HttpConn *conn, char *requestedEtag);
PUBLIC bool httpMatchModified(HttpConn *conn, MprTime time);
PUBLIC bool httpProcessCompletion(HttpConn *conn);
PUBLIC void httpProcessWriteEvent(HttpConn *conn);

/********************************** HttpTx *********************************/
/*  
    Tx flags
 */
#define HTTP_TX_NO_BODY             0x1     /**< No transmission body, only send headers */
#define HTTP_TX_HEADERS_CREATED     0x2     /**< Response headers have been created */
#define HTTP_TX_SENDFILE            0x4     /**< Relay output via Send connector */
#define HTTP_TX_USE_OWN_HEADERS     0x8     /**< Skip adding default headers */
#define HTTP_TX_NO_LENGTH           0x10    /**< Don't emit a content length (used for TRACE) */

/** 
    Http Tx
    @description The tx object controls the transmission of data. This may be client requests or responses to
        client requests. Most of the APIs in the Response group still take a HttpConn object as their first parameter. 
        This is to make the API easier to remember - APIs take a connection object rather than a rx or 
        transmission object.
    @defgroup HttpTx HttpTx
    @see HttpConn HttpRx HttpTx httpAddHeader httpAddHeaderString httpAppendHeader httpAppendHeaderString httpFinalize
    httpConnect httpCreateTx httpDestroyTx httpFinalize httpFlush httpFollowRedirects httpFormatBody httpFormatError
    httpFormatErrorV httpFormatResponse httpFormatResponseBody httpFormatResponsev httpGetQueueData
    httpIsChunked httpIsComplete httpIsOutputFinalized httpNeedRetry httpOmitBody httpRedirect httpRemoveHeader
    httpSetContentLength httpSetContentType httpSetCookie httpSetEntityLength httpSetHeader httpSetHeaderString
    httpSetResponded httpSetStatus httpSocketBlocked httpWait httpWriteHeaders httpWriteUploadData 
    @stability Internal
 */
typedef struct HttpTx {
    /* Ordered for debugging */
    int             finalized;              /**< Request response generated and handler processing is complete */
    int             pendingFinalize;        /**< Call httpFinalize again once the Tx pipeline is created */
    int             finalizedConnector;     /**< Connector has finished sending the response */
    int             finalizedOutput;        /**< Handler or surrogate has finished writing output response */
    char            *filename;              /**< Name of a real file being served (typically pathInfo mapped) */
    int             flags;                  /**< Response flags */
    int             status;                 /**< HTTP response status */
    int             responded;              /**< The request has started to respond. Some output has been initiated. */
    int             started;                /**< Handler has started */
    MprOff          bytesWritten;           /**< Bytes written including headers */
    MprOff          entityLength;           /**< Original content length before range subsetting */
    ssize           chunkSize;              /**< Chunk size to use when using transfer encoding. Zero for unchunked. */
    cchar           *ext;                   /**< Filename extension */
    char            *etag;                  /**< Unique identifier tag */
    HttpStage       *handler;               /**< Final handler serving the request */
    MprOff          length;                 /**< Transmission content length */
    int             writeBlocked;           /**< Transmission writing is blocked */
    HttpUri         *parsedUri;             /**< Client request uri */
    char            *method;                /**< Client request method GET, HEAD, POST, DELETE, OPTIONS, PUT, TRACE */
    cchar           *errorDocument;         /**< Error document to render */

    struct HttpConn *conn;                  /**< Current connection object */
    MprList         *outputPipeline;        /**< Output processing */
    HttpStage       *connector;             /**< Network connector to send / receive socket data */
    HttpQueue       *queue[2];              /**< Pipeline queue heads */

    MprHash         *headers;               /**< Transmission headers */
    HttpCache       *cache;                 /**< Cache control entry (only set if this request is being cached) */
    MprBuf          *cacheBuffer;           /**< Response caching buffer */
    ssize           cacheBufferLength;      /**< Current size of the cache buffer data */
    cchar           *cachedContent;         /**< Retrieved cached response to send */

    HttpRange       *outputRanges;          /**< Data ranges for tx data */
    HttpRange       *currentRange;          /**< Current range being fullfilled */
    char            *rangeBoundary;         /**< Inter-range boundary */
    MprOff          rangePos;               /**< Current range I/O position in response data */

    char            *altBody;               /**< Alternate transmission for errors */
    int             traceMethods;           /**< Handler methods supported */

    /* File information for file-based handlers */
    MprFile         *file;                  /**< File to be served */
    MprPath         fileInfo;               /**< File information if there is a real file to serve */
    ssize           headerSize;             /**< Size of the header written */

    char            *webSockKey;            /**< Sec-WebSocket-Key header */
} HttpTx;

/** 
    Add a header to the transmission using a format string.
    @description Add a header if it does not already exits.
    @param conn HttpConn connection object created via #httpCreateConn
    @param key Http response header key
    @param fmt Printf style formatted string to use as the header key value
    @param ... Arguments for fmt
    @return "Zero" if successful, otherwise a negative MPR error code. Returns MPR_ERR_ALREADY_EXISTS if the header already
        exists.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpAddHeader(HttpConn *conn, cchar *key, cchar *fmt, ...);

/** 
    Add a header to the transmission
    @description Add a header if it does not already exits.
    @param conn HttpConn connection object created via #httpCreateConn
    @param key Http response header key
    @param value Value to set for the header
    @return Zero if successful, otherwise a negative MPR error code. Returns MPR_ERR_ALREADY_EXISTS if the header already
        exists.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpAddHeaderString(HttpConn *conn, cchar *key, cchar *value);

/** 
    Append a transmission header
    @description Set the header if it does not already exists. Append with a ", " separator if the header already exists.
    @param conn HttpConn connection object created via #httpCreateConn
    @param key Http response header key
    @param fmt Printf style formatted string to use as the header key value
    @param ... Arguments for fmt
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpAppendHeader(HttpConn *conn, cchar *key, cchar *fmt, ...);

/** 
    Append a transmission header string
    @description Set the header if it does not already exists. Append with a ", " separator if the header already exists.
    @param conn HttpConn connection object created via #httpCreateConn
    @param key Http response header key
    @param value Value to set for the header
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpAppendHeaderString(HttpConn *conn, cchar *key, cchar *value);

/** 
    Indicate the request is finalized.
    @description Calling this routine indicates that the handler has fully finished processing the request including
        generating a full response and any other required processing. This call will invoke #httpFinalizeOutput and 
        then set the request finalized flag. If the request is already finalized, this call does nothing. 
        A handler MUST call httpFinalize when it has completed processing a request.
    @param conn HttpConn connection object
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpFinalize(HttpConn *conn);

/** 
    Connect to a server and issue Http client request.
    @description Start a new Http request on the http object and return. This routine does not block.
        After starting the request, you can use #httpWait to wait for the request to achieve a certain state or to complete.
    @param conn HttpConn connection object created via #httpCreateConn
    @param method Http method to use. Valid methods include: "GET", "POST", "PUT", "DELETE", "OPTIONS" and "TRACE" 
    @param uri URI to fetch
    @param ssl SSL configuration to use if a secure connection. 
    @return "Zero" if the request was successfully sent to the server. Otherwise a negative MPR error code is returned.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC int httpConnect(HttpConn *conn, cchar *method, cchar *uri, struct MprSsl *ssl);

/** 
    Create the tx object. This is used internally by the http library.
    @param conn HttpConn connection object created via #httpCreateConn
    @param headers Optional headers to use for the transmission
    @returns A tx object
    @ingroup HttpTx
    @stability Internal
 */
PUBLIC HttpTx *httpCreateTx(HttpConn *conn, MprHash *headers);

/**
    Destroy the tx object
    @description This is called when the garbage collector frees a connection. It should not be called manually.
    @param tx Tx object
    @ingroup HttpTx
    @stability Internal
 */
PUBLIC void httpDestroyTx(HttpTx *tx);

/** 
    Finalize transmission of the http response
    @description This routine will force the transmission of buffered content to the peer. It should be called by clients
    and Handlers to signify the end of the body content being sent with the request or response body. 
    HttpFinalize will set the finalizedOutput flag and write a final chunk trailer if using chunked transfers. If the
    request output is already finalized, this call does nothing.  Note that after finalization, incoming content may
    continue to be processed.
    i.e. httpFinalize can be called before all incoming data has been received. 
    @param conn HttpConn connection object
    @ingroup HttpTx
    @stability Evolving
 */
PUBLIC void httpFinalizeOutput(HttpConn *conn);

/**
    Flush tx data. This writes any buffered data. 
    @param conn HttpConn connection object created via #httpCreateConn
    @ingroup HttpTx
    @stability Evolving
 */
PUBLIC void httpFlush(HttpConn *conn);

/** 
    Follow redirctions
    @description Enabling follow redirects enables the Http service to transparently follow 301 and 302 redirections
        and fetch the redirected URI.
    @param conn HttpConn connection object created via #httpCreateConn
    @param follow Set to true to enable transparent redirections
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpFollowRedirects(HttpConn *conn, bool follow);

/** 
    Format an error transmission
    @description Format an error message to use instead of data generated by the request processing pipeline.
        This is typically used to send errors and redirections. The message is also sent to the error log.
    @param conn HttpConn connection object created via #httpCreateConn
    @param status Http response status code
    @param fmt Printf style formatted string. This string may contain HTML tags and is not HTML encoded before
        sending to the user. NOTE: Do not send user input back to the client using this method. Otherwise you open
        large security holes.
    @param ... Arguments for fmt
    @return A count of the number of bytes in the transmission body.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpFormatError(HttpConn *conn, int status, cchar *fmt, ...);

/** 
    Format an alternate response
    @description Format a response to use instead of data generated by the request processing pipeline.
        This is used for alternate responses that are not errors.
    @param conn HttpConn connection object created via #httpCreateConn
    @param fmt Printf style formatted string. This string may contain HTML tags and is not HTML encoded before
        sending to the user. NOTE: Do not send user input back to the client using this method. Otherwise you open
        large security holes.
    @param ... Arguments for fmt
    @return A count of the number of bytes in the transmission body.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC ssize httpFormatResponse(HttpConn *conn, cchar *fmt, ...);

/** 
    Format an alternate response
    @description Format a response to use instead of data generated by the request processing pipeline.
        This is similar to #httpFormatResponse.
    @param conn HttpConn connection object created via #httpCreateConn
    @param fmt Printf style formatted string. This string may contain HTML tags and is not HTML encoded before
        sending to the user. NOTE: Do not send user input back to the client using this method. Otherwise you open
        large security holes.
    @param args Varargs style list of arguments
    @return A count of the number of bytes in the transmission body.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC ssize httpFormatResponsev(HttpConn *conn, cchar *fmt, va_list args);

/** 
    Format a response body.
    @description Format a transmission body to use instead of data generated by the request processing pipeline.
        The body will be created in HTML or in plain text depending on the value of the request Accept header.
        This call is used for alternate responses that are not errors.
    @param conn HttpConn connection object created via #httpCreateConn
    @param title Title string to format into the HTML transmission body.
    @param fmt Printf style formatted string. This string may contain HTML tags and is not HTML encoded before
        sending to the user. NOTE: Do not send user input back to the client using this method. Otherwise you open
        large security holes.
    @param ... Arguments for fmt
    @return A count of the number of bytes in the transmission body.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC ssize httpFormatResponseBody(HttpConn *conn, cchar *title, cchar *fmt, ...);

/**
    Get the queue data for the connection
    @param conn HttpConn connection object created via #httpCreateConn
    @return the private queue data object
 */
PUBLIC void *httpGetQueueData(HttpConn *conn);

/** 
    Return whether transfer chunked encoding will be used on this request
    @param conn HttpConn connection object created via #httpCreateConn
    @returns true if chunk encoding will be used
    @ingroup HttpTx
    @stability Stable
 */ 
PUBLIC int httpIsChunked(HttpConn *conn);

/**
    Test if request has been finalized
    @description This call tests if #httpFinalize has been called.
    @param conn HttpConn connection object
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC int httpIsFinalized(HttpConn *conn);

/**
    Test if request response has been fully generated.
    @description This call tests if all transmit data has been generated and finalized. Handlers call #httpFinalizeOutput
        to signify the end of transmit data.
    @param conn HttpConn connection object
    @ingroup HttpTx
    @stability Evolving
 */
PUBLIC int httpIsOutputFinalized(HttpConn *conn);

/** 
    Determine if the transmission needs a transparent retry to implement authentication or redirection. This is used
    by client requests. If authentication is required, a request must first be tried once to receive some authentication 
    key information that must be resubmitted to gain access.
    @param conn HttpConn connection object created via #httpCreateConn
    @param url Reference to a string to receive a redirection URL. Set to NULL if not redirection is required.
    @return true if the request needs to be retried.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC bool httpNeedRetry(HttpConn *conn, char **url);

/**
    Tell the tx to omit sending any body
    @param conn HttpConn connection object created via #httpCreateConn
 */
PUBLIC void httpOmitBody(HttpConn *conn);

/** 
    Redirect the client
    @description Redirect the client to a new uri.
    @param conn HttpConn connection object created via #httpCreateConn
    @param status Http status code to send with the response
    @param uri New uri for the client
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpRedirect(HttpConn *conn, int status, cchar *uri);

/** 
    Remove a header from the transmission
    @description Remove a header if present.
    @param conn HttpConn connection object created via #httpCreateConn
    @param key Http response header key
    @return "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC int httpRemoveHeader(HttpConn *conn, cchar *key);

/** 
    Define a content length header in the transmission. This will define a "Content-Length: NNN" request header and
        set Tx.length.
    @param conn HttpConn connection object created via #httpCreateConn
    @param length Numeric value for the content length header.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpSetContentLength(HttpConn *conn, MprOff length);

/** 
    Set the transmission (response) content mime type
    @description Set the mime type Http header in the transmission
    @param conn HttpConn connection object created via #httpCreateConn
    @param mimeType Mime type string
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpSetContentType(HttpConn *conn, cchar *mimeType);

/*
    Flags for httpSetCookie
 */
#define HTTP_COOKIE_SECURE   0x1         /**< Flag for httpSetCookie for secure cookies (https only) */
#define HTTP_COOKIE_HTTP     0x2         /**< Flag for httpSetCookie for http cookies (http only) */

/** 
    Set a transmission cookie
    @description Define a cookie to send in the transmission Http header
    @param conn HttpConn connection object created via #httpCreateConn
    @param name Cookie name
    @param value Cookie value
    @param path URI path to which the cookie applies
    @param domain Domain in which the cookie applies. Must have 2-3 dots.
    @param lifespan Duration for the cookie to persist in msec
    @param flags Cookie options mask. The following options are supported:
        @li HTTP_COOKIE_SECURE    - Set the 'Secure' attribute on the cookie.
        @li HTTP_COOKIE_HTTP      - Set the 'HttpOnly' attribute on the cookie.
        See RFC 6265 for details about the 'Secure' and 'HttpOnly' cookie attributes.
    @ingroup HttpTx
    @stability Evolving
 */
PUBLIC void httpSetCookie(HttpConn *conn, cchar *name, cchar *value, cchar *path, cchar *domain, 
        MprTicks lifespan, int flags);

/**
    Define the length of the transmission content. When static content is used for the transmission body, defining
    the entity length permits the request pipeline to know when all the data has been sent.
    @param conn HttpConn connection object created via #httpCreateConn
    @param len Transmission body length in bytes
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpSetEntityLength(HttpConn *conn, MprOff len);

/** 
    Set a transmission header
    @description Set a Http header to send with the request. If the header already exists, it its value is overwritten.
    @param conn HttpConn connection object created via #httpCreateConn
    @param key Http response header key
    @param fmt Printf style formatted string to use as the header key value
    @param ... Arguments for fmt
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpSetHeader(HttpConn *conn, cchar *key, cchar *fmt, ...);

/** 
    Set a simple key/value transmission header
    @description Set a Http header to send with the request. If the header already exists, it its value is overwritten.
    @param conn HttpConn connection object created via #httpCreateConn
    @param key Http response header key
    @param value String value for the key
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpSetHeaderString(HttpConn *conn, cchar *key, cchar *value);

/** 
    Set a Http response status.
    @description Set the Http response status for the request. This defaults to 200 (OK).
    @param conn HttpConn connection object created via #httpCreateConn
    @param status Http status code.
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpSetStatus(HttpConn *conn, int status);

/**
    Set the responded flag for the request
    @description This call sets the requests responded status. Once status has been defined, headers generated or 
        some output has been generated, the request is regarded as having "responded" in-part to the client.
    @param conn HttpConn connection object
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpSetResponded(HttpConn *conn);

/**
    Indicate that the transmission socket is blocked
    @param conn Http connection object created via #httpCreateConn
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpSocketBlocked(HttpConn *conn);

/** 
    Wait for the connection to achieve the requested state.
    @description This call blocks until the connection reaches the desired state. It creates a wait handler and
        services any events while waiting. This is useful for blocking client requests.
    @param conn HttpConn connection object created via #httpCreateConn
    @param state HTTP_STATE_XXX to wait for.
    @param timeout Timeout in milliseconds to wait. Set to -1 to use the default inactivity timeout. Set to zero
        to wait for ever.
    @return "Zero" if successful. Otherwise return a negative MPR error code. Specific returns include:
        MPR_ERR_TIMEOUT and MPR_ERR_BAD_STATE.
    @ingroup HttpTx
    @stability Evolving
 */
PUBLIC int httpWait(HttpConn *conn, int state, MprTicks timeout);

/** 
    Write the transmission headers into the given packet
    @description Write the Http transmission headers into the given packet. This should only be called by connectors
        just prior to sending output to the client. It should be delayed as long as possible if the content length is
        not yet known to give the pipeline a chance to determine the transmission length. This way, a non-chunked 
        transmission can be sent with a content-length header. This is the fastest HTTP transmission.
    @param q Queue owning the packet
    @param packet Packet into which to place the headers
    @ingroup HttpTx
    @stability Stable
 */
PUBLIC void httpWriteHeaders(HttpQueue *q, HttpPacket *packet);

/** 
    Write Http upload body data
    @description Write files and form fields as request body data. This will use transfer chunk encoding. This routine 
        will block until all the buffer is written even if a callback is defined.
    @param conn Http connection object created via #httpCreateConn
    @param fileData List of string file names to upload
    @param formData List of strings containing "key=value" pairs. The form data should be already www-urlencoded.
    @return Number of bytes successfully written.
    @ingroup HttpConn
    @stability Stable
 */
PUBLIC ssize httpWriteUploadData(HttpConn *conn, MprList *formData, MprList *fileData);

/********************************* HttpEndpoint ***********************************/
/*  
    Endpoint flags
 */
#define HTTP_NAMED_VHOST    0x1             /**< Using named virtual hosting */
#define HTTP_NEW_DISPATCHER 0x2             /**< New dispatcher for each connection */

/** 
    Listening endpoints. Endpoints may have multiple virtual named hosts.
    @defgroup HttpEndpoint HttpEndpoint
    @see HttpEndpoint httpAcceptConn httpAddHostToEndpoint httpCreateConfiguredEndpoint httpCreateEndpoint 
        httpDestroyEndpoint httpGetEndpointContext httpHasNamedVirtualHosts httpIsEndpointAsync
        httpLookupHostOnEndpoint httpSecureEndpoint httpSecureEndpointByName httpSetEndpointAddress 
        httpSetEndpointAsync httpSetEndpointContext httpSetEndpointNotifier httpSetHasNamedVirtualHosts 
        httpStartEndpoint httpStopEndpoint httpValidateLimits 
    @stability Internal
 */
typedef struct HttpEndpoint {
    Http            *http;                  /**< Http service object */
    MprList         *hosts;                 /**< List of host objects */
    HttpLimits      *limits;                /**< Alias for first host, default route resource limits */
    MprHash         *clientLoad;            /**< Table of active client IPs and connection counts */
    char            *ip;                    /**< Listen IP address. May be null if listening on all interfaces. */
    int             port;                   /**< Listen port */
    int             async;                  /**< Listening is in async mode (non-blocking) */
    int             activeClients;          /**< Count of current active clients */
    int             activeRequests;         /**< Count of current active requests */
    int             flags;                  /**< Endpoint control flags */
    void            *context;               /**< Embedding context */
    MprSocket       *sock;                  /**< Listening socket */
    MprDispatcher   *dispatcher;            /**< Event dispatcher */
    HttpNotifier    notifier;               /**< Default connection notifier callback */
    struct MprSsl   *ssl;                   /**< Endpoint SSL configuration */
    MprMutex        *mutex;                 /**< Multithread sync */
} HttpEndpoint;

/**
    Accept a new connection.
    Accept a new client connection on a new socket. If multithreaded, this will come in on a worker thread 
        dedicated to this connection. This is called from the listen wait handler.
    @param endpoint The endpoint on which the server was listening
    @param event Mpr event object
    @return A HttpConn object representing the new connection.
    @ingroup HttpEndpoint
    @stability Internal
    @internal
 */
PUBLIC HttpConn *httpAcceptConn(HttpEndpoint *endpoint, MprEvent *event);

/**
    Add a host to an endpoint
    @description Add the host to the endpoint's list  of hosts. A listening endpoint may have multiple
        virutal hosts.
    @param endpoint Endpoint to which the host will be added.
    @param host HttpHost object to add.
    @return "Zero" if the host can be added.
    @ingroup HttpEndpoint
    @stability Internal
 */
PUBLIC void httpAddHostToEndpoint(HttpEndpoint *endpoint, struct HttpHost *host);

/**
    Create and configure a new endpoint.
    @description Convenience function to create and configure a new endpoint without using a config file.
        An endpoint is created with a default host and default route.
    @param home Home directory for configuration files for the endpoint 
    @param documents Directory containing the 
    @param ip IP address to use for the endpoint. Set to null to listen on all interfaces.
    @param port Listening port number to use for the endpoint
    @return A configured HttpEndpoint object instance
    @ingroup HttpEndpoint
    @stability Internal
*/
PUBLIC HttpEndpoint *httpCreateConfiguredEndpoint(cchar *home, cchar *documents, cchar *ip, int port);

/** 
    Create an endpoint  object.
    @description Creates a listening endpoint on the given IP:PORT. Use httpStartEndpoint to begin listening for client
        connections.
    @param ip IP address on which to listen
    @param port IP port number
    @param dispatcher Dispatcher to use. Can be null.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC HttpEndpoint *httpCreateEndpoint(cchar *ip, int port, MprDispatcher *dispatcher);

/**
    Destroy the endpoint
    @description This destroys the endpoint created by #httpCreateEndpoint. Calling this routine should not
        normally be necessary as the garbage collector will invoke as required.
    @param endpoint HttpEndpoint object returned from #httpCreateEndpoint.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC void httpDestroyEndpoint(HttpEndpoint *endpoint);

/**
    Get the endpoint context object
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @return The endpoint context object defined via httpSetEndpointContext
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC void *httpGetEndpointContext(HttpEndpoint *endpoint);

/**
    Test if an endpoint has named virtual hosts.
    @param endpoint Endpoint object to examine
    @return True if the endpoint has named virutal hosts.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC bool httpHasNamedVirtualHosts(HttpEndpoint *endpoint);

/**
    Get if the endpoint is running in asynchronous mode
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @return True if the endpoint is in async mode
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC int httpIsEndpointAsync(HttpEndpoint *endpoint);

/**
    Lookup a host name
    @description Lookup a host by name in the set of defined hosts for this endpoint.
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @param name Host name to search for
    @return An HttpHost object instance or null if the host cannot be found.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC struct HttpHost *httpLookupHostOnEndpoint(HttpEndpoint *endpoint, cchar *name);

/**
    Secure an endpoint 
    @description Define the SSL parameters for an endpoint. This must be done before starting listening on
        the endpoint via #httpStartEndpoint.
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @param ssl MprSsl object
    @returns "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC int httpSecureEndpoint(HttpEndpoint *endpoint, struct MprSsl *ssl);

/**
    Secure an endpoint by name
    @description Define the SSL parameters for an endpoint that is selected by name. This must be done before 
    starting listening on the endpoint via #httpStartEndpoint.
    @param name Endpoint name. The endpoint name is comprised of the IP and port. For example: "127.0.0.1:7777" 
    @param ssl MprSsl object
    @returns Zero if successful, otherwise a negative MPR error code.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC int httpSecureEndpointByName(cchar *name, struct MprSsl *ssl);

/**
    Set the endpoint IP address
    @description This call defines the endpoint's IP address and port number. If the endpoint has already been
        started, this will stop and restart the endpoint. Current requests will not be disturbed.
        This is useful to modify the endpoints address when using dynamically assigned IP addresses.
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @param ip IP address to use for the endpoint. Set to null to listen on all interfaces.
    @param port Listening port number to use for the endpoint
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC void httpSetEndpointAddress(HttpEndpoint *endpoint, cchar *ip, int port);

/**
    Control if the endpoint is running in asynchronous mode
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @param enable Set to 1 to enable async mode.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC void httpSetEndpointAsync(HttpEndpoint *endpoint, int enable);

/**
    Set the endpoint context object
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @param context New context object
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC void httpSetEndpointContext(HttpEndpoint *endpoint, void *context);

/** 
    Define a notifier callback for this endpoint.
    @description The notifier callback will be invoked as Http requests are processed.
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @param fn Notifier function. 
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC void httpSetEndpointNotifier(HttpEndpoint *endpoint, HttpNotifier fn);

/**
    Control whether the endpoint has named virtual hosts.
    @param endpoint Endpoint object to examine
    @param on Set to true to enable named virtual hosts
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC void httpSetHasNamedVirtualHosts(HttpEndpoint *endpoint, bool on);

/** 
    Start listening for client connections.
    @description Opens the endpoint socket and starts listening for connections.
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @returns "Zero" if successful, otherwise a negative MPR error code.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC int httpStartEndpoint(HttpEndpoint *endpoint);

/** 
    Stop the server listening for client connections.
    @description Closes the socket endpoint.
    @param endpoint HttpEndpoint object created via #httpCreateEndpoint
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC void httpStopEndpoint(HttpEndpoint *endpoint);

/**
    Validate the request against defined resource limits
    @description The Http library supports a suite of resource limits that restrict the impact of a request on
        the system. This call validates a processing event for the current request against the server's endpoint
        limits.
    @param endpoint The endpoint on which the server was listening
    @param event Processing event. The supported events are: HTTP_VALIDATE_OPEN_CONN, HTTP_VALIDATE_CLOSE_CONN,
        HTTP_VALIDATE_OPEN_REQUEST and HTTP_VALIDATE_CLOSE_REQUEST.
    @param conn HttpConn connection object
    @return True if the request can be successfully validated against the endpoint limits.
    @ingroup HttpEndpoint
    @stability Stable
 */
PUBLIC bool httpValidateLimits(HttpEndpoint *endpoint, int event, HttpConn *conn);

/********************************** HttpHost ***************************************/
/*
    Flags
 */
#define HTTP_HOST_VHOST         0x1         /**< Host flag to signify host is a virtual host */
#define HTTP_HOST_NAMED_VHOST   0x2         /**< Host flag for a named virtual host */
#define HTTP_HOST_NO_TRACE      0x10        /**< Host flag to disable the of TRACE HTTP method */

/**
    Host Object
    @description A Host object represents a logical host. Several logical hosts may share a single HttpEndpoint.
    @defgroup HttpHost HttpHost
    @see HttpHost httpAddRoute httpCloneHost httpCreateHost httpResetRoutes httpSetHostHome httpSetHostIpAddr 
        httpSetHostName httpSetHostProtocol
    @stability Internal
*/
typedef struct HttpHost {
    /*
        NOTE: the ip:port names are used for vhost matching when there is only one such address. Otherwise a host may
        be associated with multiple listening endpoints. In that case, the ip:port will store only one of these addresses 
        and will not be used for matching.
     */
    char            *name;                  /**< Host name */
    char            *ip;                    /**< Hostname/ip portion parsed from name */
    int             port;                   /**< Port address portion parsed from name */
    struct HttpHost *parent;                /**< Parent host to inherit aliases, dirs, routes */
    MprCache        *responseCache;         /**< Response content caching store */
    MprList         *routes;                /**< List of Route defintions */
    HttpRoute       *defaultRoute;          /**< Default route for the host */
    HttpEndpoint    *defaultEndpoint;       /**< Default endpoint for host */
    HttpEndpoint    *secureEndpoint;        /**< Secure endpoint for host */
    char            *protocol;              /**< Defaults to "HTTP/1.1" */
    char            *root;                  /**< Home for this host */
    int             flags;                  /**< Host flags */
    MprMutex        *mutex;                 /**< Multithread sync */
} HttpHost;

/**
    Add a route to a host
    @description Add the route to the host list of routes. During request route matching, routes are processed 
    in order, so it is important to define routes in the order in which you wish to match them.
    @param host HttpHost object
    @param route Route to add
    @return "Zero" if the route can be added.
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC int httpAddRoute(HttpHost *host, HttpRoute *route);

/**
    Clone a host
    @description The parent host is cloned and a new host returned. The new host inherites the parent's configuration.
    @param parent Parent HttpHost object to clone
    @return The new HttpHost object.
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC HttpHost *httpCloneHost(HttpHost *parent);

/**
    Create a host
    @description Create a new host object. The host is added to the Http service's list of hosts.
    @return The new HttpHost object.
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC HttpHost *httpCreateHost();

/**
    Get the default host defined via httpSetDefaultHost
    @return The defaul thost object
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC HttpHost *httpGetDefaultHost();

/**
    Get the default route for a host
    @param host Host object
    @return The default route for the host
    @ingroup HttpRoute
    @stability Stable
 */
PUBLIC HttpRoute *httpGetDefaultRoute(HttpHost *host);

/**
    Return the default route for a host
    @description The host has a default route which holds default configuration. Typically the default route
        is not directly used when routing URIs. Rather other routes inherit from the default route and are used to 
        respond to client requests.
    @param host Host to examine.
    @return Default route object
    @ingroup HttpRoute
    @stability Stable
 */
PUBLIC HttpRoute *httpGetHostDefaultRoute(HttpHost *host);

/**
    Show the current route table to the error log.
    @description This emits the currently defined route table for a host to the route table. If the "full" argument is true,
        a more-complete, multi-line output format will be used. Othewise, a one-line, abbreviated route description will
        be output.
    @param host Host to examine.
    @param full Set to true for a "fuller" output route description.
    @ingroup HttpRoute
    @stability Stable
 */
PUBLIC void httpLogRoutes(HttpHost *host, bool full);

/**
    Lookup a route by name
    @param host HttpHost object owning the route table
    @param name Route name to find. If null or empty, look for "default"
    @ingroup HttpRoute
    @stability Stable
 */
PUBLIC HttpRoute *httpLookupRoute(HttpHost *host, cchar *name);

/**
    Reset the list of routes for the host
    @param host HttpHost object
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpResetRoutes(HttpHost *host);

/**
    Set the default host for all servers.
    @param host Host to define as the default host
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpSetDefaultHost(HttpHost *host);

/**
    Set the default endpoint for a host
    @description The host may have a default endpoint that is used when doing redirections to http.
    @param host Host to examine.
    @param endpoint Secure endpoint to use as the default
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpSetHostDefaultEndpoint(HttpHost *host, HttpEndpoint *endpoint);

/**
    Set the default route for a host
    @description The host has a default route which holds default configuration. Typically the default route
        is not directly used when routing URIs. Rather other routes inherit from the default route and are used to 
        respond to client requests.
    @param host Host to examine.
    @param route Route to define as the default
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpSetHostDefaultRoute(HttpHost *host, HttpRoute *route);

/**
    Set the default secure endpoint for a host
    @description The host may have a default secure endpoint that is used when doing redirections to https.
    @param host Host to examine.
    @param endpoint Secure endpoint to use as the default
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpSetHostSecureEndpoint(HttpHost *host, HttpEndpoint *endpoint);

/**
    Set the server root for a host
    @description The server root is used as the default directory to locate configuration files for the host
    @param host HttpHost object
    @param root Directory path for the host server root
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpSetHostRoot(HttpHost *host, cchar *root);

/**
    Set the host internet address
    @description Set the host IP and port address.
    @param host HttpHost object
    @param ip Internet address. This can be an IP address or a symbolic domain and host name.
    @param port Port number 
    @return "Zero" if the route can be added.
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpSetHostIpAddr(HttpHost *host, cchar *ip, int port);

/**
    Set the host name
    @description The host name is used when matching virtual hosts with the Http Host header. The host name is also
        used for some redirections.
    in order, so it is important to define routes in the order in which you wish to match them.
    @param host HttpHost object
    @param name Host name to use
    @return Zero if the route can be added.
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpSetHostName(HttpHost *host, cchar *name);

/**
    Set the host HTTP protocol version
    @description Set the host protocol version to either HTTP/1.0 or HTTP/1.1
    @param host HttpHost object
    @param protocol Set to either HTTP/1.0 or HTTP/1.1
    @ingroup HttpHost
    @stability Stable
 */
PUBLIC void httpSetHostProtocol(HttpHost *host, cchar *protocol);

/*
    Internal
 */
PUBLIC int httpStartHost(HttpHost *host);
PUBLIC void httpStopHost(HttpHost *host);

/********************************* Web Sockets *************************************/
/** 
    WebSocket Service to implement the WebSockets RFC 6455 specification for client and server communications.
    @description WebSockets is a technology providing interactive communication between a server and client. Normal HTML
    connections follow a request / response paradigm and do not easily support asynchronous communications or unsolicited
    data pushed from the server to the client. WebSockets solves this by supporting bi-directional, full-duplex
    communications over persistent connections. A WebSocket connection is established over a standard HTTP connection and is
    then upgraded without impacting the original connection. This means it will work with existing networking infrastructure
    including firewalls and proxies.
    @defgroup HttpWebSocket HttpWebSocket
    @see httpGetWebSocketCloseReason httpGetWebSocketMessageLength httpGetWebSocketProtocol httpGetWriteQueueCount
        httpIsLastPacket httpSend httpSendBlock httpSendClose httpSetWebSocketProtocols httpWebSocketOrderlyClosed
    @stability Internal
 */
typedef struct HttpWebSocket {
    int             state;                  /**< State */
    int             frameState;             /**< Message frame state */
    int             closing;                /**< Started closing sequnce */
    int             closeStatus;            /**< Close status provided by peer */
    ssize           frameLength;            /**< Length of the current frame */
    ssize           messageLength;          /**< Length of the current message */
    char            *subProtocol;           /**< Application level sub-protocol */
    HttpPacket      *currentFrame;          /**< Pending message frame */
    HttpPacket      *currentMessage;        /**< Pending message frame */
    MprEvent        *pingEvent;             /**< Ping timer event */
    char            *closeReason;           /**< Reason for closure */
    uchar           dataMask[4];            /**< Mask for data */
    int             maskOffset;             /**< Offset in dataMask */
} HttpWebSocket;

#define WS_VERSION     13
#define WS_MAGIC       "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

/*
    httpSendBlock message types
 */
#define WS_MSG_CONT     0x0       /**< Continuation of WebSocket message */
#define WS_MSG_TEXT     0x1       /**< httpSendBlock type for text messages */
#define WS_MSG_BINARY   0x2       /**< httpSendBlock type for binary messages */
#define WS_MSG_CONTROL  0x8       /**< Start of control messages */
#define WS_MSG_CLOSE    0x8       /**< httpSendBlock type for close message */
#define WS_MSG_PING     0x9       /**< httpSendBlock type for ping messages */
#define WS_MSG_PONG     0xA       /**< httpSendBlock type for pong messages */
#define WS_MSG_MAX      0xB       /**< Max message type for httpSendBlock */

/*
    Close message status codes
    0-999       Unused
    1000-1999   Reserved for spec
    2000-2999   Reserved for extensions
    3000-3999   Library use
    4000-4999   Application use
 */
#define WS_STATUS_OK                   1000     /**< Normal closure */
#define WS_STATUS_GOING_AWAY           1001     /**< Endpoint is going away. Server down or browser navigating away */
#define WS_STATUS_PROTOCOL_ERROR       1002     /**< WebSockets protocol error */
#define WS_STATUS_UNSUPPORTED_TYPE     1003     /**< Unsupported message data type */
#define WS_STATUS_FRAME_TOO_LARGE      1004     /**< Message frame is too large */
#define WS_STATUS_NO_STATUS            1005     /**< No status was received from the peer in closing */
#define WS_STATUS_COMMS_ERROR          1006     /**< TCP/IP communications error  */
#define WS_STATUS_INVALID_UTF8         1007     /**< Text message has invalid UTF-8 */
#define WS_STATUS_POLICY_VIOLATION     1008     /**< Application level policy violation */
#define WS_STATUS_MESSAGE_TOO_LARGE    1009     /**< Message is too large */
#define WS_STATUS_MISSING_EXTENSION    1010     /**< Unsupported WebSockets extension */
#define WS_STATUS_INTERNAL_ERROR       1011     /**< Server terminating due to an internal error */
#define WS_STATUS_TLS_ERROR            1015     /**< TLS handshake error */
#define WS_STATUS_MAX                  5000     /**< Maximum error status (less one) */

/*
    WebSocket states (rx->webSockState)
 */
#define WS_STATE_CONNECTING     0               /**< WebSocket connection is being established */
#define WS_STATE_OPEN           1               /**< WebSocket handsake is complete and ready for communications */
#define WS_STATE_CLOSING        2               /**< WebSocket is closing */
#define WS_STATE_CLOSED         3               /**< WebSocket is closed */

/**
    Get the close reason supplied by the peer.
    @param conn HttpConn connection object created via #httpCreateConn
    @return The reason string supplied by the peer when closing the web socket.
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC char *httpGetWebSocketCloseReason(HttpConn *conn);

/**
    Get the message length for the current message
    @description The message length will be updated as the message frames are received. The message length is 
        only valid when the last frame has been received. See #httpIsLastPacket
    @param conn HttpConn connection object created via #httpCreateConn
    @return The size of the message.
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC ssize httpGetWebSocketMessageLength(HttpConn *conn);

/**
    Get the selected web socket protocol selected by the server
    @param conn HttpConn connection object created via #httpCreateConn
    @return The web socket protocol string
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC char *httpGetWebSocketProtocol(HttpConn *conn);

/**
    Get the WebSocket state
    @return The web socket state. Will be WS_STATE_CONNECTING, WS_STATE_OPEN, WS_STATE_CLOSING or WS_STATE_CLOSED.
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC ssize httpGetWebSocketState(HttpConn *conn);

/**
    Send a UTF-8 text message to the web socket peer
    @param conn HttpConn connection object created via #httpCreateConn
    @param fmt Printf style formatted string
    @param ... Arguments for the format
    @return Number of bytes written
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC ssize httpSend(HttpConn *conn, cchar *fmt, ...);

#define HTTP_MORE   0x1000         /**< Flag for #httpSendBlock to indicate there are more frames for this message */

/**
    Send a message of a given type to the web socket peer
    @description This call operates in blocking mode by default unless the HTTP_NON_BLOCK flag is specified. When blocking,
    the call will either accept and write all the data or it will fail, it will never return "short" with a partial write.
    The call may block for up to the inactivity timeout specified in the conn->limits->inactivityTimeout value.
    @param conn HttpConn connection object created via #httpCreateConn
    @param type Web socket message type. Choose from WS_MSG_TEXT, WS_MSG_BINARY or WS_MSG_PING. 
        Use httpSendClose to send a close message. Do not send a WS_MSG_PONG message as it is generated internally
        by the Web Sockets module.
    @param buf Data buffer to send
    @param len Length of buf
    @param flags Set to HTTP_BLOCK for blocking operation or HTTP_NON_BLOCK for non-blocking. Set to HTTP_BUFFER to
        buffer the data if required and never block. Set to zero will default to HTTP_BUFFER.
    @return Number of data message bytes written. Should equal len if successful, otherwise returns a negative
        MPR error code.
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC ssize httpSendBlock(HttpConn *conn, int type, cchar *buf, ssize len, int flags);

/**
    Send a close message to the web socket peer
    @param conn HttpConn connection object created via #httpCreateConn
    @param status Web socket status
    @param reason Optional reason text message. The reason must be less than 124 bytes in length.
    @return Number of data message bytes written. Should equal len if successful, otherwise returns a negative
        MPR error code.
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC void httpSendClose(HttpConn *conn, int status, cchar *reason);

/**
    Set a list of application-level protocols supported by the client
    @param conn HttpConn connection object created via #httpCreateConn
    @param protocols Comma separated list of application-level protocols
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC void httpSetWebSocketProtocols(HttpConn *conn, cchar *protocols);

/**
    Upgrade a client HTTP connection connection to use WebSockets
    @description This requests an upgrade to use WebSockets. Note this is the upgrade request and the
        confirmation handshake response must still be received and validated. The connection must be upgraded
        before sending any data to the server.
    @param conn HttpConn connection object created via #httpCreateConn
    @return Return Zero if the connection upgrade can be requested.
    @stability Evolving
    @ingroup HttpWebSocket
    @internal
 */
PUBLIC int httpUpgradeWebSocket(HttpConn *conn);

/**
    Test if web socket connection was orderly closed by sending an acknowledged close message
    @param conn HttpConn connection object created via #httpCreateConn
    @return True if the web socket was orderly closed.
    @ingroup HttpWebSocket
    @stability Evolving
 */
PUBLIC bool httpWebSocketOrderlyClosed(HttpConn *conn);

/************************************ Misc *****************************************/
/**
    Add an option to the options table
    @param options Option table returned from httpGetOptions
    @param field Field key name
    @param value Value to use for the field
    @ingroup Http
    @stability Evolving
 */
PUBLIC void httpAddOption(MprHash *options, cchar *field, cchar *value);

/**
    Add an option to the options table. 
    @description If the field already exists, the added value is inserted prior to the existing value.
    @param options Option table returned from httpGetOptions
    @param field Field key name
    @param value Value to use for the field
    @ingroup Http
    @stability Evolving
*/
PUBLIC void httpInsertOption(MprHash *options, cchar *field, cchar *value);

/**
    Extract a field value from an option string. 
    @param options Option string of the form: "field='value' field='value'..."
    @param field Field key name
    @param defaultValue Value to use if "field" is not found in options
    @return Option value.
    @ingroup Http
    @stability Evolving
 */
PUBLIC void *httpGetOption(MprHash *options, cchar *field, cchar *defaultValue);

/**
    Get an option value that is itself an object (hash)
    @description This returns an option value that is an instance of MprHash. When deserializing a JSON option string which
    contains multiple levels, this routine can be used to extract lower option container values. 
    @param options Options object to examine.
    @param field Property to return.
    @return An MprHash instance for the given field. This will contain option sub-properties.
    @ingroup Http
    @stability Evolving
 */
PUBLIC MprHash *httpGetOptionHash(MprHash *options, cchar *field);

/**
    Convert an options string into an options table
    @param options Option string of the form: "{field:'value', field:'value'}"
        This is a sub-set of the JSON syntax. Arrays are not supported.
    @return Options table
    @ingroup Http
    @stability Evolving
 */
PUBLIC MprHash *httpGetOptions(cchar *options);

//  MOB - inconsistent with httpGetOption
/**
    Test a field value from an option string. 
    @param options Option string of the form: "field='value' field='value'..."
    @param field Field key name
    @param value Test if the field is set to this value
    @param useDefault If true and "field" is not found in options, return true
    @return Allocated value string.
    @ingroup Http
    @stability Evolving
 */
PUBLIC bool httpOption(MprHash *options, cchar *field, cchar *value, int useDefault);

/**
    Remove an option
    @description Remove a property from an options hash
    @param options Options table returned from httpGetOptions
    @param field Property field to remove
    @ingroup Http
    @stability Evolving
 */
PUBLIC void httpRemoveOption(MprHash *options, cchar *field);

/**
    Set an option
    @description Set a property in an options hash
    @param options Options table returned from httpGetOptions
    @param field Property field to set
    @param value Property value to use
    @ingroup Http
    @stability Evolving
 */
PUBLIC void httpSetOption(MprHash *options, cchar *field, cchar *value);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _h_HTTP */

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
