jQuery(document).ready(function($){
	//cache some jQuery objects
	
	var modalTrigger = $('.cd-modal-trigger'),
		transitionLayer = $('.cd-transition-layer'),
		transitionBackground = transitionLayer.children(),
		modalWindow = $('.cd-modal');

	var frameProportion = 1.78, //png frame aspect ratio
		frames = transitionLayer.data('frame'), //number of png frames
		resize = false;

	//set transitionBackground dimentions
	setLayerDimensions();
	$(window).on('resize', function(){
		if( !resize ) {
			resize = true;
			(!window.requestAnimationFrame) ? setTimeout(setLayerDimensions, 300) : window.requestAnimationFrame(setLayerDimensions);
		}
	});

	

	//open modal window
	modalTrigger.on('click',function(event){	
		event.preventDefault();
		var modalId = $(event.target).attr('href');
		console.log($(event.target));
		transitionLayer.addClass('visible opening');
		var delay = ( $('.no-cssanimations').length > 0 ) ? 0 : 800;
		setTimeout(function(){
			modalWindow.filter(modalId).addClass('visible');
			transitionLayer.removeClass('opening');
		}, delay);
	});

	//close modal window
	modalWindow.on('click', '.modal-close', function(event){
		event.preventDefault();
		transitionLayer.addClass('closing');
		modalWindow.removeClass('visible');
		transitionBackground.one('webkitAnimationEnd oanimationend msAnimationEnd animationend', function(){
			transitionLayer.removeClass('closing opening visible');
			transitionBackground.off('webkitAnimationEnd oanimationend msAnimationEnd animationend');
		});
	});

	function setLayerDimensions() {
		var windowWidth = $(window).width(),
			windowHeight = $(window).height(),
			layerHeight, layerWidth;

		if( windowWidth/windowHeight > frameProportion ) {
			layerWidth = windowWidth;
			layerHeight = layerWidth/frameProportion;
		} else {
			layerHeight = windowHeight*1.2;
			layerWidth = layerHeight*frameProportion;
		}

		transitionBackground.css({
			'width': layerWidth*frames+'px',
			'height': layerHeight+'px',
		});

		resize = false;
	}
	var HomeAnimation = function(){
		this.container = $('#home-animation-wrapper');
		this.currentSlide = 1;
		this.slidesLength = this.container.find('.slide').length - 1; // To start at 0
		this.content = { text1: '', text2: '', text3: ''};
		
		this.allowChange = true;
		
		this.init();
	  };
	  
	  HomeAnimation.prototype = {
		
		init: function(){
		  console.log('init');
		  
		  this.bind();
	  
		  this.initSvg();
		  
		},
		
		bind: function(){
		  console.log('bind');
	  
		  var self = this;
		  
		  // Bind interval
		  var timerDuration = 5000,
			  timer = setInterval(function(){
				self.resetSlide();
			  }, timerDuration);
	  
		  // Bind restart on click with next slide
		  this.container.on('click', function(){
			
			// Do not allow to change slide until last transition is complete
			if(!self.allowChange){ return; }
			self.allowChange = false;
			setTimeout(function(){
			  self.allowChange = true;
			}, 2000);
			
			self.resetSlide();
			
			clearInterval(timer);
			timer = setInterval(function(){
				self.resetSlide();
			  }, timerDuration);
	  
			
		  });
		  
	  
		},
		
		initSvg: function(){
		  console.log('initSvg');
		  
		  this.bubblesRandomColors();
		  this.setTexts();
		  // this.bubblesMovement();
		  
		  // Trigger First slide
		  this.triggerSlide();
		},
		
		setTexts: function(){
		  console.log('setTexts');
		  
		  var texts = this.container.find('#svg-texts');
		  
		  // Get texts
		  this.content.text1 = this.container.find('.slide[count="'+this.currentSlide+'"]').attr('data-1');
		  this.content.text2 = this.container.find('.slide[count="'+this.currentSlide+'"]').attr('data-2');
		  this.content.text3 = this.container.find('.slide[count="'+this.currentSlide+'"]').attr('data-3');
		  
		  // UpperCase Texts
		  this.content.text1 = this.content.text1.toUpperCase();
		  this.content.text2 = this.content.text2.toUpperCase();
		  this.content.text3 = this.content.text3.toUpperCase();
		  
		  // Append Texts
		  texts.find('text:nth-child(1)').html(this.content.text1);
		  texts.find('text:nth-child(2)').html(this.content.text2);
		  texts.find('text:nth-child(3)').html(this.content.text3);
		},
		
		bubblesRandomColors: function(){
		  console.log('bubblesRandomColors');
		  
		  var bubblesGroup = this.container.find('#bubbles'),
			   //colors = ['#03ee93','#ff0700','#393e46','#5306de'];
			  colors = ['#f8b500','#00adb5','#393e46'];
			  //colors = ['#f38181','#fce38a','#95e1d3'];
		  bubblesGroup.find('path').each(function(){
			var rand = getRand(0,colors.length);
			TweenMax.set(this, { fill: colors[rand], scale: 0, transformOrigin: 'center' });
		  })    
		},
		
		triggerSlide: function(){
		  console.log('triggerSlide');
		  
		  // Container loads invisible, needs this to be shown on the first iteration
		  TweenMax.set(this.container, { autoAlpha: 1 });
	  
		  var bubbles =  this.container.find('#bubbles path');
		  bubbles.each(function(){
			var rotateRand = getRand(-30,30);
			TweenMax.to(this,1.5, { scale: 1.2, rotation: rotateRand, ease: Power2.easeInOut});
		  })
		},
		
		resetSlide: function(){
		  console.log('hideSlide');
	  
		  var self = this;
		  
		  this.currentSlide++;
		  if(this.currentSlide > this.slidesLength){ this.currentSlide = 0; };
		  
		  var bubbles = this.container.find('#bubbles path');
		  TweenMax.to(bubbles,1.5, { scale: 0, ease: Power2.easeInOut, onComplete: triggerContinue });
		  
		  // Workaround gsap onComplete
		  function triggerContinue(){
			self.initSvg();
		  }
		},
		
		// non used function (performance)
		bubblesMovement: function(){
		  var bubbles = this.container.find('#bubbles path'),
			  count = 500;
		  
		  bubbles.each(function(index){
			if(index > count ) { return; }
			$(this).attr('data-rand', getRand(15, 25) );
		  });
		  
		  this.container.on('mousemove', function(e){
			bubbles.each(function(){
			  if(!$(this).attr('data-rand')){ return; };
			  
			  var rand = $(this).attr('data-rand'),
				  cy = e.pageY,
				  cx = e.pageX,
				  py = Math.round($(this).offset().top),
				  px = Math.round($(this).offset().left),
				  x = Math.round(-(cx + px) / rand),
				  y = Math.round(-(cy + py) / rand);
	  
			  TweenMax.to($(this), 0.2, { x: x, y: y, transformOrigin: 'center' });
			});
		  })
		}
	  }
	  
	  function getRand(min, max) {
		min = Math.ceil(min);
		max = Math.floor(max);
		return Math.floor(Math.random() * (max - min)) + min; 
	  }
	  
	  window.LLOS = window.LLOS || {};
	  if($('#home-animation-wrapper').length){
		window.LLOS.HomeAnimation = new HomeAnimation();
	  }
});

