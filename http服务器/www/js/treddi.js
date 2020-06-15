/*
 * 3D Gallery view 1.0 - jQuery plugin
 *
 * Copyright (c) 2011 Lunardi Loris - Italy
 *
 * Site: http://ajoin.it
 * e-mail: l.lunardi@ajoin.it
 *
 */
var window_loaded = false;

(function($){
		  
	$.fn.dddgallery = function(options) {
		var opts = $.extend($.fn.dddgallery.defaults,options);
		//operazioni preliminari
		var n_box = $(this).children('.box').size();
		j_obj = $(this);
		var isdrag;
		coox = new Array();
		cooy = new Array();
		coow = new Array();
		cooh = new Array();
		cool = new Array();
		cooo = new Array();
		max_spost = new Array();
		vedimove=false;
		$('.box').css('width',opts.p_width);
		global_x = $(j_obj).width();
		global_y = $(j_obj).height();
		//3° legge della dinamica
			//attendo caricamento pagina
		$(j_obj).children().hide();
		$(j_obj).append('<div id="loading"><img src="img/loader.gif"></div>');
		if(!window_loaded) {
				$(window).load(function(){
					buildtreddi();
				});
			} else {
				buildtreddi();
			}

			//ridimensionamento
		$(j_obj).resize(function (){ buildtreddi(); });
			//drag
		if(opts.drag){
			$( ".box" ).draggable({
				start: function(event, ui) {  isdrag = $(this).attr("alt"); /*se sta draggando non sente il mouseevent*/ },
				drag: function (event,ui) { $(this).children("span").html($(this).offset().left);},
				stop: function(e, ui) { 
						 	isdrag = null; 
							i = $(this).attr("alt");	
							coox [i] = ($(this).offset().left)-($(this).parent().offset().left)-((max_spost [i]*(global_x-(e.pageX-(global_x/2))))/global_x-(max_spost [i])); //imposto nuove coordinate
							cooy [i] = ($(this).offset().top)-($(this).parent().offset().top)-((max_spost [i]*(global_y-(e.pageY-(global_y/2))))/global_y-(max_spost [i]));
							$(this).children("span").html($(this).offset().left);
							}
				});
		}
			//movimento mouse
		$(j_obj).parent().mousemove(function(e){ movetreddi(e);});
			//mouse sopra
		$( ".box" ).mouseenter(function() {
				if(!$(this).hasClass('box_principale')){
					if(!isdrag){
						iv = $(this).attr("alt");
						ingrandimento_w = coow [i] + (coow [i]*opts.ingra/100); 
						ingrandimento_h = cooh [i] + (cooh [i]*opts.ingra/100); 
						var cssObj_Ent = {
							  'z-index' : opts.livel_range+3,
							  'opacity' : 1
							}
						$(this).css(cssObj_Ent).append('<div class="vedi"><b>Vedi</b></div>'); //aggiungo il pulsante per vederlo in primo piano
						//quando clicco vedi
						$('div.vedi').click( function(){vediclick(iv); });
					}
				}
			  }).mouseleave(function() {
  				if(!isdrag){
					i = $(this).attr("alt");
					var cssObj_ex = {
					  'z-index' : cool [i],
					  'opacity' : cooo [i]
					}
					//reimposto i vecchi css e rimuovo il vedi scaduto il tempo di timeout
					$(this).css(cssObj_ex);
					$('div.vedi').remove(); 										

				}
			 });
			
		

			  
		//fine eventi --||-- inizio raccolta funzioni
		
		function buildtreddi() {
			global_x = $(j_obj).width();
			global_y = $(j_obj).height();

			$('#loading').fadeOut(function(){$('#loading').remove();});
			$(j_obj).children().fadeIn(500);
		
			//calcolo altezza box_principale
			box_p_height = ($('.box_principale').height()*opts.p_width )/$('.box_principale').width();
			for(i=1; i<=n_box; i++){
				//verifico che il box se è nella classe principale e lo imposto cone le impostazioni di default
				if($(j_obj).children("div:nth-child("+i+")").hasClass('box_principale')){
					coow [i] = opts.p_width; //larghezza
					cooh [i] = box_p_height; //altezza
					cool [i] = opts.livel_range+2; //livello
					coox [i] = (global_x / 2 ) - (opts.p_width / 2); //posizione x
					cooy [i] = (global_y / 2 ) - (cooh [i] / 2); //posizione y
					cooo [i] = 1; //opacità

					
				}else{
				//imposto un livello di profondità random
					cool [i] = 1+Math.round(Math.random()*(opts.livel_range));
				//imposto una dimensione in base alla profondità
					coow [i] = opts.min_width + cool [i] * ((opts.max_width-opts.min_width)/(opts.livel_range-1));					
					cooh [i] = ($(j_obj).children("div:nth-child("+i+")").height()*coow [i] )/$(j_obj).children("div:nth-child("+i+")").width();
				//imposto la posizione radom [le finestre non devono essere celate dal box_principale e neanche devono essere fuori la finestra
					//coox [i] = Math.round(Math.random()*(global_x-(coow [i]/2)));
					coox [i] = Math.round(Math.random()*(global_x-(coow [i])));
					//controllo che non sia nascosto dal quadro centrale
					if((coox [i] > ((global_x / 2 )-(opts.p_width / 2)-(coow [i]/2)))&&(coox [i] < ((global_x / 2 )+(opts.p_width / 2)-(coow [i]/2)))){
						upordown = Math.round(Math.random());
						if (upordown == 1){
							cooy [i] = Math.round(Math.random()*((global_y / 2 )-(box_p_height / 2)-(cooh [i]/2)));
						}else{
							cooy [i] = ((global_y / 2 )+(box_p_height / 2)-(cooh [i]/2))+Math.round(Math.random()*((global_y-(cooh [i]/2))-((global_y / 2 )+(box_p_height / 2)-(cooh [i]/2))));
						}
					}else{
						cooy [i] = Math.round(Math.random()*(global_y-(cooh [i]/2)));
					}
				//imposto un'opacità proporzionale al livello
					cooo [i] = cool [i] * ((1-opts.min_opacity)/(opts.livel_range));
				}
				//se abilitate calcolo posizione ombre
				if(opts.shadow){
					sha_x = (((coox [i]+coow [i]/2) - global_x/2)*opts.dist_shadow)/(global_x/2)
					sha_y = (((cooy [i]+cooh [i]/2) - global_y/2)*opts.dist_shadow)/(global_y/2)
				}
				
				var cssObj_B = {
					  'width' : coow [i],
					  'left' : coox [i],
					  'top' : cooy [i],
					  'opacity' : cooo [i]
					}
				var cssObj_BB = {
					  'z-index' : cool [i],
					  'position' : 'absolute',
					  '-moz-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Firefox */
					  '-webkit-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Safari and Chrome */
					   'box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow
				}
/*					$.each(cssObj_B, function(index, value) { 
						  alert(index + ': ' + value); 
						});*/
				$(j_obj).children("div:nth-child("+i+")").animate(cssObj_B,500).attr("alt",i);
				$(j_obj).children("div:nth-child("+i+")").css(cssObj_BB);
				//$(j_obj).children("div:nth-child("+i+")").css(cssObj_B)
				//memorizzo la loro posizione
				max_spost [i] = opts.min_spost + (opts.livel_range-cool [i]+2) * ((opts.max_spost-opts.min_spost)/(opts.livel_range-1));;
				}
		}
		//funziona spostamento box
		function movetreddi(e) { 
			if(!vedimove){
				for(i=1; i<=n_box; i++){
					//prendo le informazioni del box
					m_x = $(j_obj).children("div:nth-child("+i+")").offset().left-($(j_obj).offset().left);
					m_y = $(j_obj).children("div:nth-child("+i+")").offset().top-($(j_obj).offset().top);				
					m_h = $(j_obj).children("div:nth-child("+i+")").height();				
					// calcolo nuove coordinate	
					n_x = coox [i]+(max_spost [i]*(global_x-(e.pageX-(global_x/2))))/global_x-(max_spost [i]);
					n_y = cooy [i]+(max_spost [i]*(global_y-(e.pageY-(global_y/2))))/global_y-(max_spost [i]);
					
					//se abilitate calcolo posizione ombre
					if(opts.shadow){
						sha_x = (((m_x+coow [i]/2) - global_x/2)*opts.dist_shadow)/(global_x/2);
						sha_y = (((m_y+m_h/2) - global_y/2)*opts.dist_shadow)/(global_y/2);
					}
					if(isdrag!=i){
						var cssObjM = {
							'left' : n_x,
							'top' : n_y,
							'-moz-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Firefox */
							'-webkit-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Safari and Chrome */
							'box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow
						}
					}else{
							var cssObjM = {
							'-moz-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Firefox */
							'-webkit-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Safari and Chrome */
							'box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow
						}
					}
	
					$(j_obj).children("div:nth-child("+i+")").css(cssObjM);
				}
			}
		}
		function vediclick(iv){
			vedimove=true;
			//calcolo la nuova altezza delprincipale
			new_cooh= (($(j_obj).children("div:nth-child("+iv+")").height())*opts.p_width)/$(j_obj).children("div:nth-child("+iv+")").width(); 
			
			//rimuovo l'attuale principale
			i_p = $(j_obj).children('.box_principale').attr('alt');
			if(i_p){
				//imposto un livello di profondità random
					cool [i_p] = 1+Math.round(Math.random()*(opts.livel_range));
				//imposto una dimensione in base alla profondità
					coow [i_p] = opts.min_width + cool [i_p] * ((opts.max_width-opts.min_width)/(opts.livel_range-1));		

				//imposto la posizione radom [le finestre non devono essere celate dal box_principale e neanche devono essere fuori la finestra
					coox [i_p] = Math.round(Math.random()*(global_x-(coow [i_p]/2)));
					//controllo che non sia nascosto dal quadro centrale
					if((coox [i_p] > ((global_x / 2 )-(opts.p_width / 2)-(coow [i_p]/2)))&&(coox [i_p] < ((global_x / 2 )+(opts.p_width / 2)-(coow [i_p]/2)))){
						upordown = Math.round(Math.random());
						if (upordown == 1){
							cooy [i_p] = Math.round(Math.random()*((global_y / 2 )-(new_cooh / 2)-(cooh [i_p]/2)));
						}else{
							cooy [i_p] = ((global_y / 2 )+(new_cooh / 2)-(cooh [i_p]/2))+Math.round(Math.random()*((global_y-(cooh [i_p]/2))-((global_y / 2 )+(new_cooh/ 2)-(cooh [i_p]/2))));
						}
					}else{
						cooy [i_p] = Math.round(Math.random()*(global_y-(cooh [i_p]/2)));
					}
				//imposto un'opacità proporzionale al livello
					cooo [i_p] = cool [i_p] * ((1-opts.min_opacity)/(opts.livel_range));
				
				
				//se abilitate calcolo posizione ombre
				if(opts.shadow){
					sha_x = (((coox [i_p]+coow [i_p]/2) - global_x/2)*opts.dist_shadow)/(global_x/2)
					sha_y = (((cooy [i_p]+cooh [i_p]/2) - global_y/2)*opts.dist_shadow)/(global_y/2)
					
				}
				var cssObjP = {
					  'width' : coow [i_p],
					  'left' : coox [i_p],
					  'top' : cooy [i_p],
					  'opacity' : cooo [i_p]
					}
				var cssObjPP = {
					  'z-index' : cool [i_p],
					  'position' : 'absolute',
					  '-moz-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Firefox */
					  '-webkit-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Safari and Chrome */
					   'box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow
					}
				
				$(j_obj).children("div:nth-child("+i_p+")").animate(cssObjP,500,function(){ $(j_obj).children("div:nth-child("+i_p+")").css(cssObjPP);}).removeClass('box_principale');
				//memorizzo la loro posizione
				max_spost [i_p] = opts.min_spost + (opts.livel_range-cool [i_p]+2) * ((opts.max_spost-opts.min_spost)/(opts.livel_range-1));

			}
			
			coow [iv] = opts.p_width; //larghezza
			cooh [iv] = new_cooh; //altezza
			cool [iv] = opts.livel_range+2; //livello
			coox [iv] = (global_x / 2 ) - (opts.p_width / 2); //posizione x
			cooy [iv] = (global_y / 2 ) - (cooh [iv] / 2); //posizione y
			cooo [iv] = 1; //opacità
			var cssObjI = {
				  'width' : coow [iv],
				  'left' : coox [iv],
				  'top' : cooy [iv],
				  'opacity' : cooo [iv]
				}
			var cssObjII = {
				  'z-index' : cool [iv],
				  'position' : 'absolute',
				  '-moz-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Firefox */
				  '-webkit-box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow, /* Safari and Chrome */
				   'box-shadow': sha_x+'px '+sha_y+'px '+opts.dim_shadow+'px '+opts.col_shadow
				}

			$(j_obj).children("div:nth-child("+iv+")").animate(cssObjI,500,function(){ $(j_obj).children("div:nth-child("+iv+")").css(cssObjII); vedimove=false; }).addClass('box_principale');
			//memorizzo la loro posizione
			max_spost [iv] = opts.min_spost + (opts.livel_range-cool [iv]+2) * ((opts.max_spost-opts.min_spost)/(opts.livel_range-1));
		}
		//Fine raccolta funzioni
	}
	
	$.fn.dddgallery.defaults = {
		//drag
		drag : true,
		
		//ombre
		shadow: true,
		dist_shadow : 30,
		dim_shadow: 10,
		col_shadow: '#111',
		
		//ingrandimento mouseover
		ingra: 5, //valore percentuale
		
		//spostamento
		max_spost: 300,
		min_spost: 5,
		
		//dimensioni box_principale
		p_width: 500,
		
		//impostazioni di default dei mini-box
		min_width: 250,
		max_width: 400,
		min_opacity: 0.2,
		livel_range: 20		
	};
	
	
	
	
})(jQuery);