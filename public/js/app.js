// Foundation JavaScript
// Documentation can be found at: http://foundation.zurb.com/docs
$(document).foundation();

var RECYCOOL = window.RECYCOOL || {};

var socket = io.connect("http://127.0.0.1");
socket.on('recycool:update', function (data) {
    var status = data.status;

    delete data['status'];

	if(status == 'power_on') {
        onPowerOn(data);
    }
    if(status == 'power_off') {
        onPowerOff();
    }
    if(status == 'top_open') {
        onTopOpen();
    }
    if(status == 'top_closed') {
        onTopClosed(data);
    }
    if(status == 'update') {
        onUpdate(data);
    }
});



jQuery(function($){
	// Variables
	var windowHeight = $(window).height(),
		windowWidth = $(window).width();


	$('input').knob({
        fgColor: "#ffffff",
        thickness: 0.1,
        displayPrevious: false,
        displayInput: false,
        readOnly: true,
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

    var $weight = $('.weight');
    var $volume = $('.volume');
    var $load = $('.bar-meter li');
    
    window.volume = 0;
    window.weight = 0;
    window.load = 0;


    var animateValue = function($el, value_from, value_to, step_callback) {
        
        $({value: value_from }).delay(300).animate({value: value_to }, {
            duration: 1000,
            easing:'swing',
            step: function() {
                var value = parseInt(this.value, 10);
                step_callback(value);
            }
            
        });


    };

    

    var activeSection = function(id) {
        $('section')
            .removeClass('active')
            .filter("#"+id).addClass('active');
    };

    window.onPowerOn = function(data) {
        activeSection('power-on');


        setTimeout(function() {
            onUpdate(data);

        }, 3000);
    };

    window.onPowerOff = function() {
        activeSection('power-off');
    };

    window.onTopOpen = function() {
        activeSection('open');
    };

    window.onTopClosed = function(data) {

        onUpdate(data);
        
    };

    window.onUpdate = function(data) {
        activeSection('recycool');

        if(data.volume != window.volume) {
            animateValue($volume, window.volume , data.volume, function(value) {
                $volume.find('input').val(value+1).trigger('change');
                $volume.find('span').text(value+1);
            });
            window.volume = data.volume;
        }
        if(data.weight != window.weight) {
            animateValue($weight, window.weight , data.weight, function(value) {
                $weight.find('input').val(value+1).trigger('change');
                $weight.find('span').text(value+1);
            });
            window.weight = data.weight;
        }
        if(data.load != window.load) {
            $load.hide();
            animateValue($load, 0 , data.load, function(value) {
                var v = parseInt(value / 10, 10);
                $load.eq(v).show();
            });
            window.load = data.load;
        }

    }




	

});