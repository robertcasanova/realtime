

var config = require('./config');
var express = require('express'),
    socketio = require('socket.io'),
    http = require('http'),
    path = require('path');

var app = express();
var socket_glob;

var global_status = "POWER OFF",
    global_weight = "0",
    global_volume = "0";

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
    res.render('index.html.twig', { server: config.server_IP, status: global_status, weight: global_weight, volume: global_volume });
});

app.get('/arduino', function(req, res) {
    //apertura/chiusura coperchio
    //quanto è pieno il bidone, se è pieno o vuoto
    //peso

    if( req.get('user-agent').match(/arduino/))  {
        var status = req.query.status || global_status,
        weight = req.query.weight || global_weight,
        volume = req.query.volume || global_volume;

        console.log('sending data to frontend client...');

        if(status != global_status) {
            global_status = status;
            io.sockets.in(req.sessionID).emit('recycool:status', { status: status });
        }
        if(weight != global_weight) {
            global_weight = weight;
            io.sockets.in(req.sessionID).emit('recycool:weight', { weight: weight });
        }
        if(volume != global_volume) {
            global_volume = volume;
            io.sockets.in(req.sessionID).emit('recycool:volume', { volume: volume });
        }

        res.send(200);
    } else {
        res.redirect('/');
    }


    

    
});


