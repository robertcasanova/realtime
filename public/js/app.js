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
			draw : function () {

                    // "tron" case
                    if(this.$.data('skin') == 'tron') {

                        var a = this.angle(this.cv)  // Angle
                            , sa = this.startAngle          // Previous start angle
                            , sat = this.startAngle         // Start angle
                            , ea                            // Previous end angle
                            , eat = sat + a                 // End angle
                            , r = true;

                        this.g.lineWidth = this.lineWidth;

                        this.o.cursor
                            && (sat = eat - 0.3)
                            && (eat = eat + 0.3);

                        if (this.o.displayPrevious) {
                            ea = this.startAngle + this.angle(this.value);
                            this.o.cursor
                                && (sa = ea - 0.3)
                                && (ea = ea + 0.3);
                            this.g.beginPath();
                            this.g.strokeStyle = this.previousColor;
                            this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, sa, ea, false);
                            this.g.stroke();
                        }

                        this.g.beginPath();
                        this.g.strokeStyle = r ? this.o.fgColor : this.fgColor ;
                        this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, sat, eat, false);
                        this.g.stroke();

                        this.g.lineWidth = 2;
                        this.g.beginPath();
                        this.g.strokeStyle = this.o.fgColor;
                        this.g.arc(this.xy, this.xy, this.radius - this.lineWidth + 1 + this.lineWidth * 2 / 3, 0, 2 * Math.PI, false);
                        this.g.stroke();

                        return false;
                    }
            }
		});
		
		var knob = classe;
		var arrivo = arrivo;
		$({value: 0}).delay(300).animate({value: 30}, {
			duration: 1000,
			easing:'swing',
			step: function() {
				$(knob).val(Math.ceil(this.value)).trigger('change');
				var valore = $(knob).val();
				$(knob).val(valore+'%');
			},
			
		});
	};

	
	

	RECYCOOL.animateDial('.dial',volume);



	

});