const express = require('express');
const path = require('path');
const cors = require('cors');
const bodyParser = require('body-parser');
const passport = require('passport');
const mongoose = require('mongoose');
const config = require('./config/config');

// Routes files
const Node = require('./routes/node');

mongoose.connect(config.database);
//on Connection
mongoose.connection.on('connected',function(){
    console.log('connected to databae' + config.database);
    
});
//on Error
mongoose.connection.on('error',function(err){
    console.log('Database error' + err);
    
});

const app = express();

//Port Nmber
const port = 3000;

// cors middleware
app.use(cors());

//set static folder
app.use (express.static(path.join(__dirname,'public')));


// body-parser middleware
app.use(bodyParser.json())

// passport middle ware
// app.use(passport.initialize());
// app.use(passport.session());
// require('./config/passport')(passport);

//Set Node
app.use('/node',Node);

//start server
app.listen(port,()=>{
    console.log('server started on port' + port);
    })