const mongoose = require('mongoose');
const bcrypt = require('bcryptjs');
const config = require('../config/config');

// Node data schema
const NodedataSchema = mongoose.Schema({
    Data:{
        type:Object
    }
})

const data = module.exports = mongoose.model('Data',NodedataSchema);

// Node schema
const NodeSchema = mongoose.Schema({
    nodeId: {
        type: String,
        require:true,
        unique: true
    },
    nodeData: [{type: mongoose.Schema.Types.ObjectId, ref: 'Data'}]
});

const Node = module.exports = mongoose.model('Node', NodeSchema);

module.exports.addNode = function (newNode, callback) {
    newNode.save(callback);
}

// Controller schema
const ControllerSchema = mongoose.Schema({
    constrollerId:{
        type:String,
        required: true,
        unique: true
    },
    constrollerName:{
        type:String,
        require:true
    },
    nodes:[{type: mongoose.Schema.Types.ObjectId, ref: 'node'}]
});

module.exports = mongoose.model('Controller',NodeSchema);