var curMenu = 0;    			//记录一级菜单的当前menu
var curView = 0;				//记录一级菜单的当前显示窗口
var sub_curMenu = 0;    		//记录二级菜单的当前menu
var sub_curView = 0;			//记录二级菜单的当前显示窗口
var position = 0;				//用于显示当前焦点的位置

var flag = 0;					//标志子菜单是否被隐藏

var pic_parent = null;
var subMenu_ul = null;

function init()
{
	pic_cur = pic_parent.getElementsByTagName("li");
	pic_parent.children[curMenu].className = "selected";
	pic_parent.children[curMenu].children[0].className = "change_color";
	
	//$("#menu-hotel").css("display","none");
	$(".text_marquee").text("智翔集团期待为您提供最好的服务！");
}

function focus()
{
	if(position == 0)
	{
		//pic_parent = document.getElementById("main_menu"); //获得UL的标签位置
		pic_cur = pic_parent.getElementsByTagName("li");
		//submenu_cur = document.getElementById("submeun_ul").getElementsByTagName("li");
		//alert(submenu_cur);
		pic_parent.children[curMenu].className = "selected";
		pic_parent.children[curMenu].children[0].className = "change_color";
		subMenu_ul.children[sub_curMenu].className = "";
		subMenu_ul.children[sub_curMenu].children[0].className = "";
		//submenu_cur[0].className = "selected_sub";
		//submenu_cur[0].children[0].className = "change_color";
		
		//alert(pic_cur.length);
		move_width = pic_cur[0].clientWidth;
		sub_move_width = subMenu_ul.children[0].clientWidth;
		//alert(pic_cur[0].offsetLeft);
		//alert(move_width);
		sub_curMenu = 0;
		sub_curView = 0;
	}
	else
	{
		pic_parent.children[curMenu].className= "";
		pic_parent.children[curMenu].children[0].className = "";
		subMenu_ul.children[0].className = "selected_sub";
		subMenu_ul.children[0].children[0].className = "change_color";
	}
}


document.onkeydown = function (oEvent) {
		oEvent = oEvent || window.event;
		var currKey = oEvent.keyCode || oEvent.which || oEvent.charCode;
		switch(currKey){
			case 37:
				left();
				break;
			case 39:
				right();
				break;
			case 38:
				up();
				break;
			case 40:
				down();
				break;
			case 13:
				enter();
				break;
			default:
				break;
		}
}


function left()
{
	if(position == 0)
	{
		if(curView == 0)
		{
			if(curMenu > 0)
			{
				curMenu--;
				ul_move_width = ul_move_width + move_width;
				$("#List1").stop(true,true).animate({left:ul_move_width + "px"},300);
			}
			if(curView == 0 && curMenu == 0) 
			{
				curView = 0;
				curMenu = 0;
			}
		}
		else
		{
			curMenu--;
			curView--;
			//alert(curMenu);
			//alert(flag);
			if(curMenu == 2)
			{
				jQuery("#menu-hotel").css("display","block");	
				flag = 0;
				//alert("aaa");
			}
			else
			{
				if(!flag)
				{
					$("#menu-hotel").css("display","none");
					flag = 1;
					//alert("asd  ");
				}
			}
		}
		pic_parent.children[curMenu].className = "selected";
		pic_parent.children[curMenu].children[0].className = "change_color"
		pic_parent.children[curMenu + 1].className= "";
		pic_parent.children[curMenu + 1].children[0].className = ""; 
	}
	if(position == 1)
	{
		if(sub_curView == 0)
		{
			if(sub_curMenu > 0)
			{
				sub_curMenu--;
				sub_ul_move_width = sub_ul_move_width + sub_move_width;
				//alert(ul_move_width);
				$("#submenu_ul").stop(true,true).animate({left:sub_ul_move_width + "px"},300);
			}
		}
		else
		{
			sub_curMenu--;
			sub_curView--;
			//alert(curMenu);
			//alert(flag);
		}
		subMenu_ul.children[sub_curMenu].className = "selected_sub";
		subMenu_ul.children[sub_curMenu].children[0].className = "change_color"
		subMenu_ul.children[sub_curMenu + 1].className= "";
		subMenu_ul.children[sub_curMenu + 1].children[0].className = "";  
	}
}


function right()
{
	if(position == 0)
	{
		if(curMenu < pic_parent.children.length - 1)
		{
			if(curView == 4)
			{
				curMenu++;
				ul_move_width = ul_move_width - move_width;
				//alert(ul_move_width);
				$("#List1").stop(true,true).animate({left:ul_move_width + "px"},300);
				//alert("sdfg");
			}
			else
			{
				curMenu++;
				curView++;
				//alert("sdfsdf");
				if(curMenu == 2)
				{	
					jQuery("#menu-hotel").css("display","block");
					//alert("1");
					flag = 0;
				}
				else
				{
					if(!flag)
					{
						//div_hidden = $(".menu-hotel").remove();
						$("#menu-hotel").css("display","none");
						flag = 1;
					}
				}
			}
			/* curMenu++;
			pic_parent.children[curMenu].className = "selected";
			pic_parent.children[curMenu].children[0].className = "change_color"
			pic_parent.children[curMenu - 1].className= "";
			pic_parent.children[curMenu - 1].children[0].className = ""; */
		}
		else
		{
			curMenu = pic_parent.children.length - 1;
		}
		pic_parent.children[curMenu].className = "selected";
		pic_parent.children[curMenu].children[0].className = "change_color"
		//alert("df");
		pic_parent.children[curMenu - 1].className= "";
		pic_parent.children[curMenu - 1].children[0].className = ""; 
		//load_text(subMenu_ul,jsonobj.length,jsonobj,curMenu + 1);
		//alert("df");
	}
	if(position == 1)
	{
		if(sub_curMenu < subMenu_ul.children.length - 1)
		{
			if(sub_curView == 4)
			{
				sub_curMenu++;
				sub_ul_move_width = sub_ul_move_width - sub_move_width;
				//alert(ul_move_width);
				$("#submenu_ul").stop(true,true).animate({left:sub_ul_move_width + "px"},300);
			}
			else
			{
				sub_curMenu++;
				sub_curView++;
			}
			/* curMenu++;
			pic_parent.children[curMenu].className = "selected";
			pic_parent.children[curMenu].children[0].className = "change_color"
			pic_parent.children[curMenu - 1].className= "";
			pic_parent.children[curMenu - 1].children[0].className = ""; */
		}
		else
		{
			sub_curMenu = subMenu_ul.children.length - 1;
		}
		subMenu_ul.children[sub_curMenu].className = "selected_sub";
		subMenu_ul.children[sub_curMenu].children[0].className = "change_color"
		subMenu_ul.children[sub_curMenu - 1].className= "";
		subMenu_ul.children[sub_curMenu - 1].children[0].className = ""; 
		//load_text(subMenu_ul,jsonobj.length,jsonobj,curMenu + 1);
		//alert("df");
	}
}


function up()
{
	if(curMenu == 2)
	{
		if(position == 1)
			position = 0;
		focus();
	}
}


function down()
{
	if(curMenu == 2)
	{
		if(position == 0)
		{
			position = 1;
			focus();
		}
	}
}


function enter()
{
	if(position == 0)
	{
		if(curMenu == 0){
			CookieUtil.set("loginStatus","yes");
			window.location.href = "collection/config.html";
		}
		if(curMenu == 1){
			CookieUtil.set("loginStatus","yes");
			window.location.href = "collection/playback.html";
		}
		if(curMenu == 3){
			CookieUtil.set("loginStatus","yes");
			window.location.href = "collection/help.html";
		}
	}
	else
	{
		if(sub_curMenu == 2){
			CookieUtil.set("loginStatus","yes");
			window.location.href = "collection/videoInfo.html";
		}
		if(sub_curMenu == 4){
			CookieUtil.set("loginStatus","yes");
			window.location.href = "collection/exception.html";
		}
	}
}


function getJsonData()
{
	$.ajax({
		type:"GET",
		url:"data.txt",
		dataType:"json",
		success:function(data){
			$("#dataTemp").text(data.temp);
			$("#dataHui").text(data.hui);
			$("#dataLight").text(data.light);
			$("#dataLed").text(data.led);
			//alert(data.temp);
		}
		/* complete:function(XMLHttpRequest){
			alert("a");
			XMLHttpRequest.open("get","/webs/www/data.txt",true);
			XMLHttpRequest.send();
			alert("b"); 
			//xmlHttp = null;
			//alert("b");
		} */
	});
	setTimeout('getJsonData()',3000);
	return false;
}


/* function checkLogin(){
	$.getJSON("../cgi-bin/login.cgi?username=&pwd=",function(data){
		alert("sdf");
		if(data.username == "" || data.pwd == ""){
			alert("请登录");
			window.location.href = "index.html";
			return false;
		}
		else{
			alert("dddd");
		}
	});	
	alert("dfsdf");
}
 */
function checkLogin(){
	//alert("cccc");
	if(CookieUtil.get("username") && CookieUtil.get("pwd")){
		return true;
	}
	else
		return false;
}
 

$(function(){
	jQuery.ajaxSetup({cache:false});
	//alert("sdfsd");
//	if(checkLogin()){
//		//CookieUtil.delete("username");
//		//CookieUtil.delete("pwd");	
//		CookieUtil.set("loginStatus","yes");	
//		pic_parent = document.getElementById("List1");
//		subMenu_ul = document.getElementById("submenu_ul");
//		init();
//		//setInterval('getJsonData()',2000);
//		getJsonData();
//	}
//	else{
//		alert("请先登录！");
//		window.location.href = "index.html";
//	}

	init();
	getJsonData();

});
