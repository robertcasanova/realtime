

var config = require('./config');
var express = require('express'),
    socketio = require('socket.io'),
    http = require('http'),
    path = require('path');

var app = express();
var socket_glob;

var prev_status = "power_off";

// Configuration
app.configure(function(){
    app.set('port', process.env.PORT || 80);
    app.set('views', __dirname + '/public');
    app.set('view engine', 'twig');
    app.set('view cache', false);
    app.disabled('view cache');
    app.set('twig options', {
        strict_variables: false,
        cache: false,
        auto_reload: true
    });
    app.use(express.favicon());
    app.use(express.logger('short'));
    app.use(express.bodyParser());
    app.use(express.methodOverride());
    app.use(app.router);
    app.use(express.static(path.join(__dirname, 'public')));
});

app.configure('development', function(){
    app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
});

app.configure('production', function(){
    app.use(express.errorHandler());
});

var server = http.createServer(app).listen(app.get('port'), function(){
    console.log("Express server listening on port "+ app.get('port') +" in "+ app.get('env') +" mode.");
});

/******************************************************************/

//Socket.io init
var io = socketio.listen(server);

// removed logging
io.configure(function() {
    io.enable('browser client minification');
    io.set('log level', 1);
});

/******************************************************************/

// Routes

app.get('/', function(req,res) {
    res.render('index.html.twig', { server: config.server_IP});
});

app.get('/arduino', function(req, res) {
    //apertura/chiusura coperchio
    //quanto è pieno il bidone, se è pieno o vuoto
    //peso

    //if( req.get('user-agent').match(/arduino/))  {
        var status = req.query.status || 'power_off',
            load = req.query.load || 0,
            weight = req.query.weight || 0,
            volume = req.query.volume || 0;


        if(status != prev_status || status == "update") {
            prev_status = status;
            io.sockets.in(req.sessionID).emit('recycool:update', { status: status, load: load, weight: weight, volume: volume });
            console.log('Data sent to clients');
        } else {
            console.log("No status changes");
        }

        prev_status = status;

        res.send(200);
   // } else {
     //   res.redirect('/');
   // }


    

    
});


