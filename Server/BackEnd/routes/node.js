const express = require('express');
const router = express.Router();
const passport = require('passport')
const jwt = require('jsonwebtoken');
const config = require('../config/config');
const mongoose = require('mongoose');
const dynamic_schemas = require('mongoose-dynamic-schemas');

const Data = require('../schema/dataschema');
const Node = require('../schema/NodeSchema');



router.post('/addNewNode',function(req,res,next){
    let newNode = new Node({
        nodeId:req.body.nodeId,
    }
    );
    Node.addNode(newNode,function(err,node){
        if (err) {
            res.json({success:false , msg:err});
        }
        else{
            res.json({success:true , msg:"Node added"});
        }
    });
});

router.post('/addData',function(req,res,next){
    console.log(req);
    var stringTime = req.body.time;
    var parts = stringTime.split(':');
    var minutes = parts[1]*60+ +parts[0];
    var time = new Date(minutes * 60 * 1000);
    console.log(time);
    
    const data = new Data({
        node0: req.body.node0,
        node1: req.body.node1,
        centralName: req.body.centralName,
        date: req.body.date,
        time: time,
        packets: req.body.packet
    });
    console.log(data);
    data    
        .save()
        .then(result => {
            console.log(result)
        })
        .catck(err => {
            console.log(err)
        })
})

router.post('/addNewCollector',function(req,res,next){
    console.log("access route");
});

router.post('/test', (req, res, next) => {
    console.log(req);
    console.log("Test Route");
    res.status(200).json({
        Msg: "Test Route..."
    });
});

//testing dynamic schema
// router.post('/addNode', (req, res, next) => {
//     const nodeNumber = req.body.nodeNumber;
//     Data
//         .then(result => {
//             dynamic_schemas.addSchemaField(Data.data, "Node_"+nodeNumber, [{ sensor_1: Number, sensor_2: Number, sensor_3: Number, sensor_4: Number }] )
//         })
//         .catch(err => {
//             if(err){
//                 res.status(500).json({
//                     state: false
//                 })
//             } else{ 
//                 res.status(200).json({
//                     state: true
//                 })
//             }
//         }) 
// })

module.exports = router;