#include "GPXParser.h"
#include "LinkedListAPI.h"

// Name: Carson Mifsud
// Date: 2021-03-11
// Description: Headers for helper functions

int waypoint_get ( List *my_waypoint_List );
int route_get ( List *my_route_List );
Waypoint *waypoint_function ( xmlNode *cur_node );
Route *route_function ( xmlNode *cur_node );
Track *track_function ( xmlNode *cur_node );
bool validator_xml ( xmlDoc *doc , char* gpxSchemaFile);
xmlDocPtr GPXtoXML ( GPXdoc* doc );
