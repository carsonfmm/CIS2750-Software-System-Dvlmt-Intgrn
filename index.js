jQuery(document).ready(function() {
    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/new_rows',
        success: function (data) {
              
            if ( data.variable8.length != 0 ) {
                function generateTableHead(table, data1) {
                    let thead = table.createTHead();
                    let row = thead.insertRow();

                    let th = document.createElement("th");
                    let text = document.createTextNode("File name (click to download)");
                    th.appendChild(text);
                    row.appendChild(th);

                    th = document.createElement("th");
                    text = document.createTextNode("Version");
                    th.appendChild(text);
                    row.appendChild(th);

                    th = document.createElement("th");
                    text = document.createTextNode("Creator");
                    th.appendChild(text);
                    row.appendChild(th);

                    th = document.createElement("th");
                    text = document.createTextNode("Number of waypoints");
                    th.appendChild(text);
                    row.appendChild(th);

                    th = document.createElement("th");
                    text = document.createTextNode("Number of routes");
                    th.appendChild(text);
                    row.appendChild(th);

                    th = document.createElement("th");
                    text = document.createTextNode("Number of tracks");
                    th.appendChild(text);
                    row.appendChild(th);

                }
                
                function generateTable(table, data1) {
                    for (let i = 0; i < data1.length; i++ ) {

                    let tableObject = JSON.parse( data1[i] );
                    let version = tableObject.version + '\0';
                    let creator = tableObject.creator + '\0';
                    let numWaypoints = tableObject.numWaypoints + '\0';
                    let numRoutes = tableObject.numRoutes + '\0';
                    let numTracks = tableObject.numTracks + '\0';

                    let row = table.insertRow();

                    let cell = row.insertCell();
                    let download_a = document.createElement("a");
                    let text = document.createTextNode(data.variable8[i]);
                    download_a.href = data.variable8[i];
                    download_a.innerhtml = data.variable8[i];
                    download_a.appendChild(text);
                    cell.appendChild(download_a);

                    cell = row.insertCell();
                    text = document.createTextNode(version);
                    cell.appendChild(text);

                    cell = row.insertCell();
                    text = document.createTextNode(creator);
                    cell.appendChild(text);

                    cell = row.insertCell();
                    text = document.createTextNode(numWaypoints);
                    cell.appendChild(text);

                    cell = row.insertCell();
                    text = document.createTextNode(numRoutes);
                    cell.appendChild(text);

                    cell = row.insertCell();
                    text = document.createTextNode(numTracks);
                    cell.appendChild(text);

                    }
                }
              
                let table = document.querySelector("table");
                let data1 = Object.keys(data.variable7[0]);
                generateTableHead(table, data1);
                generateTable(table, data.variable7);
            } else {
                let table = document.querySelector("table");
                let h1 = document.createElement("h1");
                let text = document.createTextNode("No Files");
                h1.appendChild(text);
                table.appendChild(h1);
            }

            console.log("File Log Panel created: " + data); 

        },
        fail: function(error) {
            console.log("Could not create File Log Panel: " + error); 
        }
    });

});

jQuery(document).ready(function() {
    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/drop_down',
        success: function (data) {

            function generateTableHead(select, data1) {

                let option = document.createElement("option");
                let text = document.createTextNode("Select...");
                option.appendChild(text);
                select.appendChild(option);

                for ( let i = 0; i < data.variable9.length; i++ ) {

                    option = document.createElement("option");
                    text = document.createTextNode(data.variable9[i]);
                    option.appendChild(text);
                    select.appendChild(option);

                }

              }
              
            let select = document.querySelector("#select-container");
            let data1 = Object.keys(data.variable9);
            generateTableHead(select, data1);

            console.log("Received user input from drop down menu: " + data); 

        },
        fail: function(error) {
            console.log("Could not get user selection from drop-down menu: " + error); 
        }
    });

});

jQuery(document).ready(function() {
    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/drop_down',
        success: function (data) {

            function generateTableHead(select, data1) {

                let option = document.createElement("option");
                let text = document.createTextNode("Select...");
                option.appendChild(text);
                select.appendChild(option);

                for ( let i = 0; i < data.variable9.length; i++ ) {

                    option = document.createElement("option");
                    text = document.createTextNode(data.variable9[i]);
                    option.appendChild(text);
                    select.appendChild(option);

                }

              }
              
            let select = document.querySelector("#select-container3");
            let data1 = Object.keys(data.variable9);
            generateTableHead(select, data1);

            console.log("Received user input from drop down menu: " + data); 

        },
        fail: function(error) {
            console.log("Could not get user selection from drop-down menu: " + error); 
        }
    });

});

jQuery("#select-container").change(function() {
    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/table_drop',
        success: function (data) {

            let temp = document.getElementById('select-container');
            let current = temp.options[temp.selectedIndex].value;
              
            let table = document.querySelector("#second-table");

            let table_array = [];
            let tracker = 0;
            let route_count = 1;
            let track_count = 1;
            let tracks_array = [];
            let track_tracker = 1;
            let route_track = [];
            let total_track = 0;

            $("#second-table").empty();

            for ( let i = 0; i < data.variable9.length; i++ ) {
                if ( current == data.variable9[i] ) {
                    table_array[tracker] = data.variable9[i];
                    tracker = tracker + 1;
                    i = i + 1;
                    while ( data.variable9[i][0] == '{' ) {
                        table_array[tracker] = data.variable9[i];
                        tracker = tracker + 1;
                        i = i + 1;
                    }
                    i = data.variable9.length;
                }
            }

            let delete_id1 = document.getElementById("current-nodes1");
            let delete_id2 = document.getElementById("current-nodes2");
            let delete_id3 = document.getElementById("current-nodes3");
            let delete_id4 = document.getElementById("current-nodes4");
            let delete_id5 = document.getElementById("current-nodes5");
            let delete_head1 = document.getElementById("current-header1");
            let delete_head2 = document.getElementById("current-header2");
            let delete_head3 = document.getElementById("current-header3");
            let delete_head4 = document.getElementById("current-header4");
            let delete_head5 = document.getElementById("current-header5");

            if ( delete_id1 ) {

                delete_id1.remove(delete_id1);
                delete_id2.remove(delete_id2);
                delete_id3.remove(delete_id3);
                delete_id4.remove(delete_id4);
                delete_id5.remove(delete_id5);

            }

            if ( delete_head1 ) {

                delete_head1.remove(delete_head1);
                delete_head2.remove(delete_head2);
                delete_head3.remove(delete_head3);
                delete_head4.remove(delete_head4);
                delete_head5.remove(delete_head5);

            }

            for ( let i = 0; i < table_array.length; i++ ) {

                if ( i == 0 ) {

                    let thead = table.createTHead();
                    let row = thead.insertRow();
        
                    let th = document.createElement("th");
                    let text = document.createTextNode("Component");
                    th.id = "current-header1";
                    th.appendChild(text);
                    row.appendChild(th);
        
                    th = document.createElement("th");
                    text = document.createTextNode("Name");
                    th.id = "current-header2";
                    th.appendChild(text);
                    row.appendChild(th);
        
                    th = document.createElement("th");
                    text = document.createTextNode("Number of points");
                    th.id = "current-header3";
                    th.appendChild(text);
                    row.appendChild(th);
        
                    th = document.createElement("th");
                    text = document.createTextNode("Length");
                    th.id = "current-header4";
                    th.appendChild(text);
                    row.appendChild(th);
        
                    th = document.createElement("th");
                    text = document.createTextNode("Loop");
                    th.id = "current-header5";
                    th.appendChild(text);
                    row.appendChild(th);

                }

                let component = "";
                if ( table_array[i][(table_array[i].length)-1] == 'e' && table_array[i][(table_array[i].length)-2] == 't' && table_array[i][(table_array[i].length)-3] == 'u' && table_array[i][(table_array[i].length)-4] == 'o' && table_array[i][(table_array[i].length)-5] == 'r' ) {
                    component = "Route " + route_count;
                    route_track[total_track] = component;
                    total_track = total_track + 1;
                    route_count = route_count + 1;
                    table_array[i] = table_array[i].substr(0, (table_array[i].length)-5);
                }
                if ( table_array[i][(table_array[i].length)-1] == 'k' && table_array[i][(table_array[i].length)-2] == 'c' && table_array[i][(table_array[i].length)-3] == 'a' && table_array[i][(table_array[i].length)-4] == 'r' && table_array[i][(table_array[i].length)-5] == 't' ) {
                    component = "Track " + track_count;
                    route_track[total_track] = component;
                    total_track = total_track + 1;
                    track_count = track_count + 1;
                    table_array[i] = table_array[i].substr(0, (table_array[i].length)-5);
                    tracks_array[track_count-1] = table_array[i];
                }

                if ( table_array[i][0] == "{" && component.includes("Route") ) {
        
                    let tableObject = JSON.parse( table_array[i] );
                    let name = tableObject.name + '\0';
                    let numPoints = tableObject.numPoints + '\0';
                    let len = tableObject.len + '\0';
                    let loop = tableObject.loop + '\0';

                    let row = table.insertRow();
                    let cell = row.insertCell();
                    let text = document.createTextNode(component);
                    cell.appendChild(text);

                    cell = row.insertCell();
                    text = document.createTextNode(name);
                    cell.appendChild(text);

                    cell = row.insertCell();
                    text = document.createTextNode(numPoints);
                    cell.appendChild(text);

                    cell = row.insertCell();
                    text = document.createTextNode(len);
                    cell.appendChild(text);

                    cell = row.insertCell();
                    text = document.createTextNode(loop);
                    cell.appendChild(text);

                }

            }

            for ( let i = 0; i < tracks_array.length-1; i++ ) {

                let tableObject = JSON.parse( tracks_array[i+1] );
                let name = tableObject.name + '\0';
                let numPoints = tableObject.numPoints + '\0';
                let len = tableObject.len + '\0';
                let loop = tableObject.loop + '\0';

                let row = table.insertRow();
                let cell = row.insertCell();
                let text = document.createTextNode("Track " + track_tracker);
                cell.appendChild(text);

                cell = row.insertCell();
                text = document.createTextNode(name);
                cell.appendChild(text);

                cell = row.insertCell();
                text = document.createTextNode(numPoints);
                cell.appendChild(text);

                cell = row.insertCell();
                text = document.createTextNode(len);
                cell.appendChild(text);

                cell = row.insertCell();
                text = document.createTextNode(loop);
                cell.appendChild(text);

                track_tracker = track_tracker + 1;

            }

            $("#select-container2").empty();

            function generateTableHead(select, data1) {

                for ( let i = 1; i < table_array.length; i++ ) {

                    let tableObject = JSON.parse( table_array[i] );
                    let name = tableObject.name;

                    option = document.createElement("option");
                    text = document.createTextNode(route_track[i-1] + ": " + name);
                    option.appendChild(text);
                    select.appendChild(option);

                }

            }
              
            let select = document.querySelector("#select-container2");
            let data1 = Object.keys(table_array);
            generateTableHead(select, data1);

            console.log("GPX View Panel created: " + data); 

        },
        fail: function(error) {
            console.log("Could not create GPX View Panel: " + error); 
        }
    });

});

jQuery("#button_rename").change(function() {

    let inputVal = document.getElementById("renamer").value;

    let current = "";

    let temp = document.getElementById('select-container2');
    if ( temp.options[temp.selectedIndex] != undefined ) {
        current = temp.options[temp.selectedIndex].value;
    } else {
        console.log ("No file has been selected. Please select a file.");
        alert ("No file has been selected. Please select a file.");
    }

    let temp1 = document.getElementById('select-container');
    let current1 = temp1.options[temp1.selectedIndex].value;

    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/rename_name',
        data: {
            userInput: inputVal,
            changeName: current,
            fileChange: current1
        },
        success: function (data) {
            console.log("Received selected file: " + data); 
        },
        fail: function(error) {
            console.log("Could not get user selection from drop down menu: " + error); 
        }
    });

});

jQuery("#add_route").click(function() {

    let temp = document.getElementById('select-container3');
    let current = temp.options[temp.selectedIndex].value;

    if ( current == "Select..." ) {
        console.log ("No file has been selected. Please select a file.");
        alert ("No file has been selected. Please select a file.");
        current = "FALSE";
    }

    let temp1 = document.getElementById('route_new_name').value;

    jQuery.ajax({
        type: 'get',
        dataType: 'json', 
        url: '/add_route',
        data: {
            fileName: current,
            RouteName: temp1
        },
        success: function (data) {
            if ( current != "FALSE" ) {
                console.log("Route added: " + data);
                alert ("Route added!");
            }
        },
        fail: function(error) {
            console.log("Could not get file selection from the drop down menu: " + error); 
        }
    });

});

jQuery("#find_path").click(function() {

    let temp1 = document.getElementById('start_lat').value;

    let temp2 = document.getElementById('start_lon').value;

    let temp3 = document.getElementById('end_lat').value;

    let temp4 = document.getElementById('end_lon').value;

    let temp5 = document.getElementById('delta').value;

    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/find_path',
        data: {
            start_lat: temp1,
            start_lon: temp2,
            end_lat: temp3,
            end_lon: temp4,
            delta: temp5,
        },
        success: function (data) {

            let route_count = 1;
            let track_count = 1;

            let array = data.variable12.split("!");

            $("#third_table").empty();

            let table = document.querySelector("#third_table");

            if ( array.length-1 == 0 ) {
                let h1 = document.createElement("h1");
                let text = document.createTextNode("No Paths Between");
                h1.appendChild(text);
                table.appendChild(h1);
            } else {

            for ( let i = 0; i < array.length-1; i++ ) {

                if ( i == 0 ) {

                    let thead = table.createTHead();
                    let row = thead.insertRow();
        
                    let th = document.createElement("th");
                    let text = document.createTextNode("Component");
                    th.id = "current-header1";
                    th.appendChild(text);
                    row.appendChild(th);
        
                    th = document.createElement("th");
                    text = document.createTextNode("Name");
                    th.id = "current-header2";
                    th.appendChild(text);
                    row.appendChild(th);

                    th = document.createElement("th");
                    text = document.createTextNode("Number of points");
                    th.id = "current-header3";
                    th.appendChild(text);
                    row.appendChild(th);

                    th = document.createElement("th");
                    text = document.createTextNode("Length");
                    th.id = "current-header4";
                    th.appendChild(text);
                    row.appendChild(th);

                    th = document.createElement("th");
                    text = document.createTextNode("Loop");
                    th.id = "current-header5";
                    th.appendChild(text);
                    row.appendChild(th);

                }

                let row = table.insertRow();

                if ( array[i] == "Route " ) {

                    let cell = row.insertCell();
                    let text = document.createTextNode(array[i]+route_count);
                    cell.appendChild(text);
                    route_count = route_count + 1;

                } else {

                    cell = row.insertCell();
                    text = document.createTextNode(array[i]+track_count);
                    cell.appendChild(text);
                    track_count = track_count + 1;

                }

                let cell = row.insertCell();
                let text = document.createTextNode(array[i+1]);
                cell.appendChild(text);

                cell = row.insertCell();
                text = document.createTextNode(array[i+2]);
                cell.appendChild(text);

                cell = row.insertCell();
                text = document.createTextNode(array[i+3]);
                cell.appendChild(text);

                cell = row.insertCell();
                text = document.createTextNode(array[i+4]);
                cell.appendChild(text);

                i = i + 1;
                i = i + 1;
                i = i + 1;
                i = i + 1;

            }

        }

        console.log("Path Between table successfully created: " + data); 

        },
        fail: function(error) {
            console.log("Could not create table for Path Between: " + error); 
        }
    });

});

jQuery("#add_waypoint").click(function() {

    let temp2 = document.getElementById('new_waypoint_lat').value;

    if ( !(temp2 >= -90 && temp2 <= 90) ) {
        console.log ("The latitude must be in the following range: -90 to 90");
        alert ("The latitude must be in the following range: -90 to 90");
        temp2 = "FALSE";
    }

    let temp3 = document.getElementById('new_waypoint_lon').value;

    if ( !(temp3 >= -180 && temp3 <= 180) ) {
        console.log ("The longitude must be in the following range: -180 to 180");
        alert ("The longitude must be in the following range: -180 to 180");
        temp3 = "FALSE";
    }

    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/add_waypoint',
        data: {
            RouteLat: temp2,
            RouteLon: temp3
        },
        success: function (data) {
            if ( temp2 != "FALSE" && temp3 != "FALSE" ) {
                console.log("Waypoint entered: " + data);
                alert ("Waypoint saved!");
            }
        },
        fail: function(error) {
            console.log("Could not get input from waypoint entry: " + error); 
        }
    });

});

jQuery("#button_other_data").click(function() {

    let current = "";

    let temp = document.getElementById('select-container2');
    if ( temp.options[temp.selectedIndex] != undefined ) {
        current = temp.options[temp.selectedIndex].value;
    } else {
        console.log ("No file has been selected. Please select a file.");
        alert ("No file has been selected. Please select a file.");
    }

    let temp1 = document.getElementById('select-container');
    let current1 = temp1.options[temp1.selectedIndex].value;

    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/get_other_data',
        data: {
            changeName: current,
            fileChange: current1
        },
        success: function (data) {

            let my_array = [];

            let alert_message = "";

            for ( let i = 0; i < data.variable12.length; i++ ) {

                my_array = data.variable12.split("!");

            }

            for ( let i = 0; i < my_array.length-1; i++ ) {
                let tableObject = JSON.parse( my_array[i] );
                let name = tableObject.name;
                let value = tableObject.value;

                alert_message = "Name of otherData: "+name+"\nValue of otherData: "+value+"\n";
            }

            if ( my_array.length == 0 ) {
                alert_message = "No otherData";
            }

            console.log ( alert_message );
            alert ( alert_message );

            console.log("OtherData successfully alerted to user: " + data); 

        },
        fail: function(error) {
            console.log("Could not get otherData name and description: " + error); 
        }
    });

});

jQuery("#final_button").click(function() {

    let select = document.getElementById("file_name_new").value;

    if ( select[(select.length)-1] == 'x' && select[(select.length)-2] == 'p' && select[(select.length)-3] == 'g' && select[(select.length)-4] == '.' ) {
        
    } else {
        console.log ("No file created. A GPX Document must end in the \".gpx\" extension.");
        alert ("No file created. A GPX Document must end in the \".gpx\" extension.");
        select = "FALSE";
    }

    jQuery.ajax({
        type: 'get',
        dataType: 'json',
        url: '/create_doc',
        data: {
            current: select
        },
        success: function (data) {
            if ( select == "FALSE" ) {
                console.log("Could not create a new GPX File: " + data);
            } else {
                console.log("New GPX File created: " + data);
            }

        },
        fail: function(error) {
            console.log("Could not create a new GPX File: " + error); 
        }
    });

});