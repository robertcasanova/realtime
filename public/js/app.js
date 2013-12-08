// Foundation JavaScript
// Documentation can be found at: http://foundation.zurb.com/docs
$(document).foundation();

var RECYCOOL = window.RECYCOOL || {};

var socket = io.connect("http://127.0.0.1");
socket.on('recycool:status', function (data) {
	console.log(data.status);
	alert("status");
});

socket.on('recycool:weight', function(data) {
	console.log(data.weight);
	alert("status");
});

socket.on('recycool:volume', function(data) {
	volume = data.volume;
	RECYCOOL.animateDial('.dial',volume);

	//volume from 0% to 100%
});

jQuery(function($){

	// Variables
	var windowHeight = $(window).height(),
		windowWidth = $(window).width();

	RECYCOOL.animateDial = function(classe, arrivo){

		$(classe).knob({
			readOnly: true,
			bgColor: '#fff',
			fgColor: '#f8c114'
		});
		
		var knob = classe;
		var arrivo = arrivo;
		$({value: 0}).delay(300).animate({value: arrivo}, {
			duration: 1000,
			easing:'swing',
			step: function() {
				$(knob).val(Math.ceil(this.value)).trigger('change');
				var valore = $(knob).val();
				$(knob).val(valore+'%');
			}
		});
	};

	
	

	RECYCOOL.animateDial('.dial',volume);



	

});