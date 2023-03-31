'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

//Sample endpoint

let sharedLib = ffi.Library('./parser/sharedLib', {
  'createValidGPXdocAndFillTableInfo' : [ 'string', [ 'string', 'string' ] ],
  'getAllInfoOfGPX' : [ 'string', [ 'string', 'string' ] ],
  'getAllInfoOfGPXName' : [ 'string', [ 'string', 'string' ] ],
  'changeTheNameofGPX' : [ 'int', [ 'string', 'string', 'string', 'string' ] ],
  'getOtherDataElement' : [ 'string', [ 'string', 'string', 'string' ] ],
  'JSONtoGPX_create' : [ 'int', [ 'string', 'string', 'string' ] ],
  'addRouteToGPX' : [ 'int', [ 'string', 'string', 'string', 'string' ] ],
  'pathFindReturn' : [ 'string', [ 'string', 'string', 'float', 'float', 'float', 'float', 'float' ] ],
});

app.get('/new_rows', function(req , res){

  let filenames = fs.readdirSync("uploads");
let j = 0;

let doc_info = null;
let full_doc_info = "";
let full_doc_info_table = "";
let tableObject = null;
let version = null;
let creator = "";
let files = "";
let JSON_files = [];
let JSON_files_names = [];
let numWaypoints = null;
let numRoutes = null;
let numTracks = null;
let tracker = 0;
let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  res.send(
    {
      variable7: JSON_files,
      variable8: JSON_files_names
    }
  );

});

app.get('/drop_down', function(req , res){

  let filenames = fs.readdirSync("uploads");
let j = 0;

let doc_info = null;
let full_doc_info = "";
let full_doc_info_table = "";
let tableObject = null;
let version = null;
let creator = "";
let files = "";
let JSON_files = [];
let JSON_files_names = [];
let numWaypoints = null;
let numRoutes = null;
let numTracks = null;
let tracker = 0;
let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  res.send(
    {
      variable9: JSON_files_names
    }
  );

});

app.get('/table_drop', function(req , res){

  let filenames = fs.readdirSync("uploads");
let j = 0;

let doc_info = null;
let full_doc_info = "";
let full_doc_info_table = "";
let tableObject = null;
let version = null;
let creator = "";
let files = "";
let JSON_files = [];
let JSON_files_names = [];
let numWaypoints = null;
let numRoutes = null;
let numTracks = null;
let tracker = 0;
let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  res.send(
    {
      variable9: my_array
    }
  );

});

app.get('/rename_name', function(req , res){

  let filenames = fs.readdirSync("uploads");
  let j = 0;
  let doc_info = null;
  let full_doc_info = "";
  let full_doc_info_table = "";
  let tableObject = null;
  let version = null;
  let creator = "";
  let files = "";
  let JSON_files = [];
  let JSON_files_names = [];
  let numWaypoints = null;
  let numRoutes = null;
  let numTracks = null;
  let tracker = 0;
  let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  let check = sharedLib.changeTheNameofGPX( "./uploads/"+req.query.fileChange, "parser/gpx.xsd", req.query.userInput, req.query.changeName );

  res.send(
    {
      variable11: my_array2
    }
  );

});

app.get('/create_gpx_doc', function(req , res){

  let filenames = fs.readdirSync("uploads");
let j = 0;

let doc_info = null;
let full_doc_info = "";
let full_doc_info_table = "";
let tableObject = null;
let version = null;
let creator = "";
let files = "";
let JSON_files = [];
let JSON_files_names = [];
let numWaypoints = null;
let numRoutes = null;
let numTracks = null;
let tracker = 0;
let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  let my_version = req.query.version;
  let my_creator = req.query.creator;

  let JSONtoGPX = "{\"version\":"+my_version+",\"creator\":\""+my_creator+"\"}";



  res.send(
    {
      variable13: my_array2
    }
  );

});

app.get('/get_other_data', function(req , res){

  let filenames = fs.readdirSync("uploads");
let j = 0;

let doc_info = null;
let full_doc_info = "";
let full_doc_info_table = "";
let tableObject = null;
let version = null;
let creator = "";
let files = "";
let JSON_files = [];
let JSON_files_names = [];
let numWaypoints = null;
let numRoutes = null;
let numTracks = null;
let tracker = 0;
let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  let checker = sharedLib.getOtherDataElement( "uploads/"+req.query.fileChange, "parser/gpx.xsd", req.query.changeName );

  res.send(
    {
      variable12: checker
    }
  );

});

let all_waypoints = "";

app.get('/add_waypoint', function(req , res){

  let filenames = fs.readdirSync("uploads");
let j = 0;

let doc_info = null;
let full_doc_info = "";
let full_doc_info_table = "";
let tableObject = null;
let version = null;
let creator = "";
let files = "";
let JSON_files = [];
let JSON_files_names = [];
let numWaypoints = null;
let numRoutes = null;
let numTracks = null;
let tracker = 0;
let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  if ( !(req.query.RouteLat == "FALSE" || req.query.RouteLon == "FALSE") ) {
    all_waypoints = all_waypoints +  "{\"lat\":"+req.query.RouteLat+",\"lon\":"+req.query.RouteLon+"}";
  }

  res.send(
    {
      variable12: all_waypoints
    }
  );

});

app.get('/add_route', function(req , res){

  let route_string = "{\"name\":\""+req.query.RouteName+"\"}";

  let checker = "";

  if ( !(req.query.fileName == "FALSE") ) {
    checker = sharedLib.addRouteToGPX( "./uploads/"+req.query.fileName, "parser/gpx.xsd", route_string, all_waypoints );
    route_string = "";
    all_waypoints = "";
  }

  res.send(
    {
      variable12: checker
    }
  );

});

app.get('/find_path', function(req , res){

  let filenames = fs.readdirSync("uploads");
let j = 0;

let doc_info = null;
let full_doc_info = "";
let full_doc_info_table = "";
let tableObject = null;
let version = null;
let creator = "";
let files = "";
let JSON_files = [];
let JSON_files_names = [];
let numWaypoints = null;
let numRoutes = null;
let numTracks = null;
let tracker = 0;
let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  let final_for_chart = "";

  for ( let i = 0; i < JSON_files_names.length; i++ ) {

    if ( ( sharedLib.pathFindReturn ( "uploads/"+JSON_files_names[i], "parser/gpx.xsd", req.query.start_lat, req.query.start_lon, req.query.end_lat, req.query.end_lon, req.query.delta) ) != "" ) {

      final_for_chart = final_for_chart + sharedLib.pathFindReturn ( "uploads/"+JSON_files_names[i], "parser/gpx.xsd", req.query.start_lat, req.query.start_lon, req.query.end_lat, req.query.end_lon, req.query.delta );
    
    }
  
  }

  res.send(
    {
      variable12: final_for_chart
    }
  );

});

app.get('/create_doc', function(req , res){

  let filenames = fs.readdirSync("uploads");
let j = 0;

let doc_info = null;
let full_doc_info = "";
let full_doc_info_table = "";
let tableObject = null;
let version = null;
let creator = "";
let files = "";
let JSON_files = [];
let JSON_files_names = [];
let numWaypoints = null;
let numRoutes = null;
let numTracks = null;
let tracker = 0;
let long_files = "";

for ( let i = 0; i < filenames.length; i++ ) {

  if ( filenames[i][(filenames[i].length)-1] == 'x' && filenames[i][(filenames[i].length)-2] == 'p' && filenames[i][(filenames[i].length)-3] == 'g' && filenames[i][(filenames[i].length)-4] == '.' ) {

    doc_info = sharedLib.createValidGPXdocAndFillTableInfo( "uploads/" + filenames[i], "parser/gpx.xsd" );

    if ( doc_info != null ) {

      long_files = long_files + filenames[i] + "!";

      JSON_files[tracker] = doc_info;

      JSON_files_names[tracker] = filenames[i];

      tableObject = JSON.parse( doc_info );
      version = version + tableObject.version + '\0';
      creator = creator + tableObject.creator + '\0';
      numWaypoints = numWaypoints + tableObject.numWaypoints + '\0';
      numRoutes = numRoutes + tableObject.numRoutes + '\0';
      numTracks = numTracks + tableObject.numTracks + '\0';
      files = files + filenames[i] + '\0';
    
      tracker = tracker + 1;

    }

  }

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info = full_doc_info + JSON_files_names[i] + "!";
  full_doc_info = full_doc_info + sharedLib.getAllInfoOfGPX( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

for ( let i = 0; i < JSON_files_names.length; i++ ) {

  full_doc_info_table = full_doc_info_table + sharedLib.getAllInfoOfGPXName( "uploads/" + JSON_files_names[i], "parser/gpx.xsd" );

}

let my_array2 = [];

for ( let i = 0; i < full_doc_info_table.length; i++ ) {

  my_array2 = full_doc_info_table.split("!");

}

let my_array = [];

for ( let i = 0; i < full_doc_info.length; i++ ) {

  my_array = full_doc_info.split("!");

}

  let final_string = "{\"version\":1.1,\"creator\":\"Carson Mifsud\"}";

  if ( req.query.current != "FALSE" ) {
    let checker = sharedLib.JSONtoGPX_create ( final_string, req.query.current, "parser/gpx.xsd" );
  }

  res.send(
    {
      variable14: my_array
    }
  );

});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
