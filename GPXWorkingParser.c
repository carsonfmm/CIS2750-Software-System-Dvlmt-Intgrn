#include "GPXParser.h"
#include "LinkedListAPI.h"
#include "GPXParserHelpers.h"

// Name: Carson Mifsud
// Date: 2021-03-11
// Description: A parser for the GPX format. Program also includes several functions to track the amount of certain tags and attributes present in the given GPX file.

int getNumWaypoints(const GPXdoc* doc) {

	if ( doc == NULL ) {
		return 0;
	}

	int num_waypoints = 0;

	num_waypoints = getLength ( doc->waypoints );

	return num_waypoints;

}

int getNumRoutes(const GPXdoc* doc) {

	if ( doc == NULL ) {
		return 0;
	}

	int num_routes = 0;

	num_routes = getLength ( doc->routes );

	return num_routes;

}

int getNumTracks(const GPXdoc* doc) {

	if ( doc == NULL ) {
		return 0;
	}

	int num_tracks = 0;

	num_tracks = getLength ( doc->tracks );

	return num_tracks;

}

int getNumSegments(const GPXdoc* doc) {

	if ( doc == NULL ) {
		return 0;
	}

	int num_segments = 0;

	ListIterator track_iterator = createIterator(doc->tracks);

	Track *my_track = nextElement( &track_iterator );

	while ( my_track != NULL ) {

		num_segments = num_segments + getLength(my_track->segments);

		my_track = nextElement( &track_iterator );
	}

	return num_segments;

}

int waypoint_get ( List *my_waypoint_List ) {

	if ( my_waypoint_List == NULL ) {
		return 0;
	}

	int num_data = 0;
	ListIterator waypoint_iterator = createIterator(my_waypoint_List);
	Waypoint *my_waypoint = nextElement( &waypoint_iterator );

	while ( my_waypoint != NULL ) {

		num_data = num_data + getLength(my_waypoint->otherData);

		if ( strcmp ( my_waypoint->name, "" ) != 0 ) {
			num_data = num_data + 1;
		}

		my_waypoint = nextElement( &waypoint_iterator );

	}

	return num_data;

}

int route_get ( List *my_route_List ) {

	if ( my_route_List == NULL ) {
		return 0;
	}

	int num_data = 0;
	ListIterator route_iterator = createIterator(my_route_List);
	Route *my_route = nextElement( &route_iterator );

	while ( my_route != NULL ) {

		num_data = num_data + getLength(my_route->otherData);

		if ( strcmp ( my_route->name, "" ) != 0 ) {
			num_data = num_data + 1;
		}

		num_data = num_data + waypoint_get ( my_route->waypoints );

		my_route = nextElement( &route_iterator );

	}

	return num_data;

}

int getNumGPXData(const GPXdoc* doc) {

	if ( doc == NULL ) {
		return 0;
	}

	int num_data = 0;
	void *temp = NULL;

	ListIterator track_iterator = createIterator(doc->tracks);

	num_data = num_data + waypoint_get ( doc->waypoints );

	num_data = num_data + route_get ( doc->routes );

	while ( (temp = nextElement(&track_iterator)) != NULL ) {

		Track *my_track = (Track *) temp;

		num_data = num_data + getLength ( my_track->otherData );

		if ( strcmp ( my_track->name, "" ) != 0 ) {
			num_data = num_data + 1;
		}

		void *temp2 = NULL;

		ListIterator track_segment_iterator = createIterator(my_track->segments);

		while ( (temp2 = nextElement(&track_segment_iterator)) != NULL ) {

			TrackSegment *my_trackSegment = (TrackSegment *) temp2;

			void *temp3 = NULL;

			ListIterator track_segment_waypoint_iterator = createIterator(my_trackSegment->waypoints);

			while ( (temp3 = nextElement(&track_segment_waypoint_iterator)) != NULL ) {

				Waypoint *my_trackSegment_waypoint = (Waypoint *) temp3;

				num_data = num_data + getLength ( my_trackSegment_waypoint->otherData );

				if ( strcmp ( my_trackSegment_waypoint->name, "" ) != 0 ) {
					num_data = num_data + 1;
				}

			}

		}

	}

	return num_data;

}

Waypoint* getWaypoint(const GPXdoc* doc, char* name) {

	if ( doc == NULL ) {
		return NULL;
	}

	ListIterator waypoint_iterator = createIterator(doc->waypoints);

	Waypoint *my_waypoint = nextElement( &waypoint_iterator );

	while ( my_waypoint != NULL ) {

		if ( strcmp( my_waypoint->name, name ) == 0 ) {

			return my_waypoint;

		}

		my_waypoint = nextElement( &waypoint_iterator );

	}

	return NULL;

}

Route* getRoute(const GPXdoc* doc, char* name) {

	if ( doc == NULL ) {
		return NULL;
	}

	ListIterator route_iterator = createIterator(doc->routes);

	Route *my_route = nextElement( &route_iterator );

	while ( my_route != NULL ) {

		if ( strcmp( my_route->name, name ) == 0 ) {

			return my_route;

		}

		my_route = nextElement( &route_iterator );

	}

	return NULL;

}

Track* getTrack(const GPXdoc* doc, char* name) {

	if ( doc == NULL ) {
		return NULL;
	}

	ListIterator track_iterator = createIterator(doc->tracks);

	Track *my_track = nextElement( &track_iterator );

	while ( my_track != NULL ) {

		if ( strcmp( my_track->name, name ) == 0 ) {

			return my_track;

		}

		my_track = nextElement( &track_iterator );

	}

	return NULL;

}

Waypoint *waypoint_function ( xmlNode *cur_node ) {

	if ( cur_node == NULL ) {
		return NULL;
	}

	Waypoint *my_waypoint = ( Waypoint *) malloc ( sizeof ( Waypoint ) );

	xmlNode *temp_node = cur_node;

	for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE) {

			/* Retrieval of xmlNodeGetContent was retrieved from http://xmlsoft.org/ */
			if ( strcmp ( "name", (char *)(cur_node->name) ) == 0 ) {
				my_waypoint->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );
				char *temp_name2 = (char *) xmlNodeGetContent ( cur_node );
				int len = strlen ( temp_name2 );
				my_waypoint->name = (char *) ( malloc ( ( len ) + 1 ) );
				strcpy ( my_waypoint->name, temp_name2 );
				free ( temp_name2 );
			}
			else {

				my_waypoint->name = (char *) ( malloc ( sizeof ( char * ) + 1 ) );
				my_waypoint->name[0] = '\0';

				my_waypoint->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

				char *temp_name = (char *) xmlNodeGetContent ( cur_node );
				GPXData *my_data = malloc ( sizeof ( GPXData ) + strlen(temp_name) * sizeof ( char *) + 1 );
				strcpy ( my_data->name, (char*)cur_node->name );
				strcpy ( my_data->value, temp_name );
				insertBack ( my_waypoint->otherData, (void *)(my_data) );
				free ( temp_name );
			}
			if ( my_waypoint->name == NULL ) {
				my_waypoint->name[0] = '\0';
			}

		}

	}

	cur_node = temp_node;

	xmlAttr *attr = NULL;
	for (attr = cur_node->properties; attr != NULL; attr = attr->next)
	{
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *cont = (char *)(value->content);

		if ( strcmp ( "lat", attrName ) == 0 ) {
			double parsed_lat = strtod ( cont, NULL );
			my_waypoint->latitude = parsed_lat;
		}
		else if ( strcmp ( "lon", attrName ) == 0 ) {
			double parsed_lon = strtod ( cont, NULL );
			my_waypoint->longitude = parsed_lon;
		}

	}

	return ( void * )my_waypoint;

}

Route *route_function ( xmlNode *cur_node ) {

	if ( cur_node == NULL ) {
		return NULL;
	}

	Route *my_route = ( Route *) malloc ( sizeof ( Route ) );
	Waypoint *my_waypoint;

	bool check = false;

	my_route->waypoints = initializeList ( &waypointToString, &deleteWaypoint, &compareWaypoints );

	my_route->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

	for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE) {

			/* Retrieval of xmlNodeGetContent was retrieved from http://xmlsoft.org/ */
			if ( strcmp ( "name", (char *)(cur_node->name) ) == 0 ) {
				char *temp_name2 = (char *) xmlNodeGetContent ( cur_node );
				int len = strlen ( temp_name2 );
				my_route->name = (char *) ( malloc ( ( len ) + 1 ) );
				strcpy ( my_route->name, temp_name2 );
				free ( temp_name2 );
				check = true;
			}
			else if ( strcmp ( "rtept", (char *)(cur_node->name) ) == 0 ) {

				xmlNode *temp_node = cur_node;
				xmlNode *temp_node2 = cur_node;

				int name_check = 0;
				int otherData_check = 0;

				for (temp_node = temp_node->children; temp_node != NULL; temp_node = temp_node->next) {
					if ( strcmp ( (char *)temp_node->name, "name" ) == 0 ) {
						name_check++;
					} else if (!( strcmp ( (char *)temp_node->name, "text" ) == 0 )) {
						otherData_check++;
					}
				}

				if ( name_check == 0 ) {

					my_waypoint = ( Waypoint *) malloc ( sizeof ( Waypoint ) );

					my_waypoint->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

					int len = sizeof ( char );
					my_waypoint->name = (char *) ( malloc ( ( len ) + 1 ) );
					my_waypoint->name[0] = '\0';

					if ( otherData_check != 0 ) {
						xmlNode *temp_node7 = cur_node;
						temp_node7 = temp_node7->children;
						temp_node7 = temp_node7->next;
						char *temp_name = (char *) xmlNodeGetContent ( temp_node7 );
						GPXData *my_data = malloc ( sizeof ( GPXData ) + strlen(temp_name) * sizeof ( char *) + 1 );
						strcpy ( my_data->name, (char*)temp_node7->name );
						strcpy ( my_data->value, temp_name );
						insertBack ( my_waypoint->otherData, (void *)(my_data) );
						free ( temp_name );
					}

					xmlAttr *attr = NULL;
					for (attr = cur_node->properties; attr != NULL; attr = attr->next)
					{
						xmlNode *value = attr->children;
						char *attrName = (char *)attr->name;
						char *cont = (char *)(value->content);

						if ( strcmp ( "lat", attrName ) == 0 ) {
							double parsed_lat = strtod ( cont, NULL );
							my_waypoint->latitude = parsed_lat;
						}
						else if ( strcmp ( "lon", attrName ) == 0 ) {
							double parsed_lon = strtod ( cont, NULL );
							my_waypoint->longitude = parsed_lon;
						}

					}

					insertBack ( my_route->waypoints, (void *)(my_waypoint) );
				}
				else {
					insertBack ( my_route->waypoints, waypoint_function ( temp_node2 ) );
				}

			}

			else {

				if ( my_route->name == NULL || strcmp ( my_route->name, "" ) == 0 ) {
					my_route->name = (char *) ( malloc ( sizeof ( char * ) + 1 ) );
					my_route->name[0] = '\0';

					my_route->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

					check = true;
				}

				char *temp_name = (char *) xmlNodeGetContent ( cur_node );
				GPXData *my_data = malloc ( sizeof ( GPXData ) + strlen(temp_name) * sizeof ( char *) + 1 );
				strcpy ( my_data->name, (char*)cur_node->name );
				strcpy ( my_data->value, temp_name );
				insertBack ( my_route->otherData, (void *)(my_data) );
				free ( temp_name );
			}

		}

	}

	if ( check == false ) {
		my_route->name = (char *) ( malloc ( sizeof ( char * ) + 1 ) );
		my_route->name[0] = '\0';
	}

	return ( void * )my_route;

}

Track *track_function ( xmlNode *cur_node ) {

	if ( cur_node == NULL ) {
		return NULL;
	}

	Track *my_track = ( Track *) malloc ( sizeof ( Track ) );

	Waypoint *my_waypoint;
	TrackSegment *my_trackSegment;

	bool check = false;

	my_track->segments = initializeList ( &trackSegmentToString, &deleteTrackSegment, &compareTrackSegments );

	my_track->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

	for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next) {

		if (cur_node->type == XML_ELEMENT_NODE) {

			/* Retrieval of xmlNodeGetContent was retrieved from http://xmlsoft.org/ */
			if ( strcmp ( "name", (char *)(cur_node->name) ) == 0 ) {
				char *temp_name2 = (char *) xmlNodeGetContent ( cur_node );
				int len = strlen ( temp_name2 );
				my_track->name = (char *) ( malloc ( ( len ) + 1 ) );
				strcpy ( my_track->name, temp_name2 );
				free ( temp_name2 );
				check = true;
			}
			else if ( strcmp ( "trkseg", (char *)(cur_node->name) ) == 0 ) {

				xmlNode *temp_node9 = cur_node->children;
				xmlNode *temp_node8;

				if ( temp_node9->next != NULL ) {

					my_trackSegment = ( TrackSegment *) malloc ( sizeof ( TrackSegment ) );
					my_trackSegment->waypoints = initializeList ( &waypointToString, &deleteWaypoint, &compareWaypoints );

					temp_node9 = temp_node9->next;

					if ( strcmp ( "trkpt", (char *)(temp_node9->name) ) == 0 ) {

						for (temp_node8 = temp_node9; temp_node8 != NULL; temp_node8 = temp_node8->next) {

							xmlNode *temp_node5 = temp_node8;

							int name_check = 0;
							int otherData_check = 0;

							for (temp_node5 = temp_node5->children; temp_node5 != NULL; temp_node5 = temp_node5->next) {
								if ( strcmp ( (char *)temp_node5->name, "name" ) == 0 ) {
									name_check++;
								} else if (!( strcmp ( (char *)temp_node5->name, "text" ) == 0 )) {
									otherData_check++;
								}
							}

							if ( name_check == 0 ) {

								my_waypoint = ( Waypoint *) malloc ( sizeof ( Waypoint ) );

								my_waypoint->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

								int len = sizeof ( char );
								my_waypoint->name = (char *) ( malloc ( ( len ) + 1 ) );
								my_waypoint->name[0] = '\0';

								if ( otherData_check != 0 ) {
									xmlNode *temp_node7 = temp_node8;
									temp_node7 = temp_node7->children;
									temp_node7 = temp_node7->next;
									char *temp_name = (char *) xmlNodeGetContent ( temp_node7 );
									GPXData *my_data = malloc ( sizeof ( GPXData ) + strlen(temp_name) * sizeof ( char *) + 1 );
									strcpy ( my_data->name, (char*)temp_node7->name );
									strcpy ( my_data->value, temp_name );
									insertBack ( my_waypoint->otherData, (void *)(my_data) );
									free ( temp_name );
								}

								xmlAttr *attr = NULL;
								for (attr = temp_node8->properties; attr != NULL; attr = attr->next)
								{
									xmlNode *value = attr->children;
									char *attrName = (char *)attr->name;
									char *cont = (char *)(value->content);

									if ( strcmp ( "lat", attrName ) == 0 ) {
										double parsed_lat = strtod ( cont, NULL );
										my_waypoint->latitude = parsed_lat;
									}
									else if ( strcmp ( "lon", attrName ) == 0 ) {
										double parsed_lon = strtod ( cont, NULL );
										my_waypoint->longitude = parsed_lon;
									}

								}

								insertBack ( my_trackSegment->waypoints, (void *)(my_waypoint) );
							}
							else {
								insertBack ( my_trackSegment->waypoints, (void *)waypoint_function ( temp_node8 ) );
							}

							temp_node8 = temp_node8->next;

						}

						insertBack ( my_track->segments, (void *) (my_trackSegment) );

					}

				}

			}
			else {

				if ( my_track->name == NULL || strcmp ( my_track->name, "" ) == 0 ) {
					my_track->name = (char *) ( malloc ( sizeof ( char * ) + 1 ) );
					my_track->name[0] = '\0';

					my_track->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

					check = true;
				}

				char *temp_name = (char *) xmlNodeGetContent ( cur_node );
				GPXData *my_data = malloc ( sizeof ( GPXData ) + strlen(temp_name) * sizeof ( char *) + 1 );
				strcpy ( my_data->name, (char*)cur_node->name );
				strcpy ( my_data->value, temp_name );
				insertBack ( my_track->otherData, (void *)(my_data) );
				free ( temp_name );
			}

		}

	}

	if ( check == false ) {
		my_track->name = (char *) ( malloc ( sizeof ( char * ) + 1 ) );
		my_track->name[0] = '\0';
	}

	return ( void * )my_track;

}

char* waypointToString( void* data ) {
	
	char* tmpStr;
	Waypoint* tmpName = (Waypoint*)data;
	int len;

	len = strlen( tmpName->name ) + (sizeof( tmpName->latitude ) * sizeof( tmpName->longitude ) * sizeof( tmpName->otherData ) * sizeof ( tmpName ) * sizeof ( Waypoint )) + 100 ;
	tmpStr = (char*) malloc ( sizeof(char)*len );

	if (data == NULL){
		return tmpStr;
	}
	
	sprintf(tmpStr, "\nWaypoint:\n\t Name: %s\n\t Longitude: %f\n\t Latitude: %f\n", tmpName->name, tmpName->longitude, tmpName->latitude );
	char *otherData_string = toString ( tmpName->otherData );
	strcat ( tmpStr, otherData_string );
	free ( otherData_string );
	
	return tmpStr;
}

void deleteRoute(void* data) {

	Route* tmpName;

	if (data == NULL){
		return;
	}

	tmpName = (Route*)data;

	free ( tmpName->name );
	freeList ( tmpName->waypoints );
	freeList ( tmpName->otherData );
	free(tmpName);
}

void deleteTrack(void* data) {

	Track* tmpName;
	
	if (data == NULL){
		return;
	}
	
	tmpName = (Track*)data;

	free ( tmpName->name );
	freeList ( tmpName->segments );
	freeList ( tmpName->otherData );
	free(tmpName);
}

char* trackToString(void* data) {

	char* tmpStr;
	Track* tmpName = (Track*)data;
	int len;

	len = strlen( tmpName->name ) + (sizeof( tmpName->otherData ) * sizeof( tmpName->segments ) * sizeof ( tmpName ) * sizeof ( Track ) ) + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );

	if (data == NULL){
		return tmpStr;
	}
	
    sprintf(tmpStr, "\nTrack:\n\t Name: %s\n", tmpName->name );
    char *otherData_string = toString ( tmpName->otherData );
    strcat ( tmpStr, otherData_string );
    char *trackSeg_string = toString ( tmpName->segments );
    strcat ( tmpStr, trackSeg_string );
    free ( otherData_string );
    free ( trackSeg_string );
	
	return tmpStr;
}

int compareTracks(const void *first, const void *second) {
	Track* tmpName1;
	Track* tmpName2;
	
	if (first == NULL || second == NULL){
		return 0;
	}
	
	tmpName1 = (Track*)first;
	tmpName2 = (Track*)second;
	
	return strcmp((char*)tmpName1->name, (char*)tmpName2->name);
}

void deleteTrackSegment(void* data) {
	TrackSegment* tmpName;
	
	if (data == NULL){
		return;
	}
	
	tmpName = (TrackSegment*)data;

	freeList ( tmpName->waypoints );

	free(tmpName);
}

char* trackSegmentToString(void* data) {

	char* tmpStr;
	TrackSegment* tmpName = (TrackSegment*)data;
	int len;

	len = sizeof( tmpName->waypoints ) * sizeof ( tmpName ) * sizeof ( TrackSegment ) * tmpName->waypoints->length + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );

	if (data == NULL){
		return tmpStr;
	}

    sprintf(tmpStr, "\nTrackSeg:\n" );
    char *otherData_string = toString ( tmpName->waypoints );
    strcat ( tmpStr, otherData_string );
    free ( otherData_string );
	
	return tmpStr;
}

int compareTrackSegments(const void *first, const void *second) {
	
	if (first == NULL || second == NULL){
		return 0;
	}

    return 1;
	
}

char* routeToString(void* data) {

	char* tmpStr;
	Route* tmpName = (Route*)data;
	int len;

	len = strlen( tmpName->name ) + (sizeof ( tmpName->waypoints ) * sizeof( tmpName->otherData ) * sizeof ( Route ) * sizeof ( tmpName ) ) + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );

	if (data == NULL){
		return tmpStr;
	}

    sprintf(tmpStr, "\nRoute:\n\t Name: %s\n", tmpName->name );
    char *otherData_string = toString ( tmpName->otherData );
    strcat ( tmpStr, otherData_string );
    char *waypoints_string = toString ( tmpName->waypoints );
    strcat ( tmpStr, waypoints_string );
    free ( otherData_string );
    free ( waypoints_string );
	
	return tmpStr;
}

int compareRoutes(const void *first, const void *second) {
	Route* tmpName1;
	Route* tmpName2;
	
	if (first == NULL || second == NULL){
		return 0;
	}
	
	tmpName1 = (Route*)first;
	tmpName2 = (Route*)second;
	
	return strcmp((char*)tmpName1->name, (char*)tmpName2->name);
}

void deleteGpxData( void* data) {

	GPXData* tmpName;
	
	if (data == NULL){
		return;
	}
	
	tmpName = (GPXData*)data;

	free(tmpName);

}
char* gpxDataToString( void* data) {

	char* tmpStr;
	GPXData* tmpName = (GPXData*)data;
	int len;

	len = ((strlen ( tmpName->name ) + strlen ( tmpName->value )) * sizeof ( GPXData )) + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );
	
	if (data == NULL){
		return tmpStr;
	}
	
	sprintf ( tmpStr, "\t otherData:\n\t\t   Name: %s\n\t\t   Value: %s\n", tmpName->name, tmpName->value );

	return tmpStr;

}

int compareGpxData(const void *first, const void *second) {

	GPXData* tmpName1;
	GPXData* tmpName2;
	
	if (first == NULL || second == NULL){
		return 0;
	}
	
	tmpName1 = (GPXData*)first;
	tmpName2 = (GPXData*)second;
	
	return strcmp((char*)tmpName1->name, (char*)tmpName2->name);

}

int compareWaypoints ( const void *first, const void *second ) {
	
	Waypoint* tmpName1;
	Waypoint* tmpName2;
	
	if (first == NULL || second == NULL){
		return 0;
	}
	
	tmpName1 = (Waypoint*)first;
	tmpName2 = (Waypoint*)second;
	
	return strcmp((char*)tmpName1->name, (char*)tmpName2->name);
}

void deleteWaypoint ( void* data ) {
	
	Waypoint* tmpName;
	
	if (data == NULL){
		return;
	}

	tmpName = (Waypoint*)data;

	free ( tmpName->name );
    freeList (tmpName->otherData);

	free(tmpName);

}

int numTracksWithLength(const GPXdoc* doc, float len, float delta) {

	if ( doc == NULL ) {
		return 0;
	}

	if ( len < 0 || delta < 0 ) {
		return 0;
	}

	int total_matches = 0;
	float return_compare = 0;

	ListIterator track_iterator = createIterator(doc->tracks);
	Track *my_track = nextElement( &track_iterator );

	while ( my_track != NULL ) {

		return_compare = getTrackLen ( my_track );

		if ( len >= (return_compare-delta) && len <= (return_compare+delta) ) {
			total_matches = total_matches + 1;
		}

		my_track = nextElement( &track_iterator );

	}

	return total_matches;

}

int numRoutesWithLength ( const GPXdoc* doc, float len, float delta ) {

	if ( doc == NULL ) {
		return 0;
	}

	if ( len < 0 || delta < 0 ) {
		return 0;
	}

	int total_matches = 0;
	float return_compare = 0;

	ListIterator route_iterator = createIterator(doc->routes);
	Route *my_route = nextElement( &route_iterator );

	while ( my_route != NULL ) {

		return_compare = getRouteLen ( my_route );

		if ( len >= (return_compare-delta) && len <= (return_compare+delta) ) {
			total_matches = total_matches + 1;
		}

		my_route = nextElement( &route_iterator );

	}

	return total_matches;

}

float getTrackLen ( const Track *tr ) {

	if ( tr == NULL ) {
		return 0;
	}

	float total_dist = 0;

	int the_length1 = tr->segments->length;

	float original_p1y, original_p1x, last_p2x, last_p2y;

	if ( tr != NULL ) {

		ListIterator segment_move = createIterator ( tr->segments );
		TrackSegment *my_segment = nextElement ( &segment_move );

		for ( int i = 0; i < the_length1; i++ ) {

			ListIterator data_waypoint_route = createIterator ( my_segment->waypoints );
			Waypoint *my_waypoint_route = nextElement ( &data_waypoint_route );

			int the_length2 = my_segment->waypoints->length;

			for ( int i = 1; i < the_length2; i++ ) {


				float p1x, p1y, p2x, p2y;

				p1y = my_waypoint_route->longitude;
				p1x = my_waypoint_route->latitude;

				my_waypoint_route = nextElement( &data_waypoint_route );

				p2y = my_waypoint_route->longitude;
				p2x = my_waypoint_route->latitude;

				float dx, dy, dz;
				
				p1y -= p2y;
				p1y *= (3.1415926536 / 180), p1x *= (3.1415926536 / 180), p2x *= (3.1415926536 / 180);
			
				dz = sin(p1x) - sin(p2x);
				dx = cos(p1y) * cos(p1x) - cos(p2x);
				dy = sin(p1y) * cos(p1x);
				total_dist = total_dist + asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

			}

			if ( i != the_length1-1 ) {

				last_p2y = my_waypoint_route->longitude;
				last_p2x = my_waypoint_route->latitude;

				my_segment = nextElement (&segment_move);
				ListIterator data_waypoint_route1 = createIterator ( my_segment->waypoints );
				Waypoint *my_waypoint_route1 = nextElement ( &data_waypoint_route1 );
				original_p1y = my_waypoint_route1->longitude;
				original_p1x = my_waypoint_route1->latitude;

				my_waypoint_route1 = nextElement( &data_waypoint_route );

				float dx, dy, dz;
				
				original_p1y -= last_p2y;
				original_p1y *= (3.1415926536 / 180), original_p1x *= (3.1415926536 / 180), last_p2x *= (3.1415926536 / 180);
			
				dz = sin(original_p1x) - sin(last_p2x);
				dx = cos(original_p1y) * cos(original_p1x) - cos(last_p2x);
				dy = sin(original_p1y) * cos(original_p1x);
				total_dist = total_dist + asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

			}

		}

	}

	total_dist = total_dist * 1000;

	return total_dist;

}

bool isLoopTrack(const Track *tr, float delta) {

	if ( tr == NULL ) {
		return false;
	}

	if ( delta < 0 ) {
		return false;
	}

	float total_dist = 0;

	int the_length1 = tr->segments->length;

	float original_p1y = 0;
	float original_p1x = 0;
	float last_p2x = 0;
	float last_p2y = 0;
	float p2x = 0;
	float p2y = 0;

	if ( tr != NULL ) {

		ListIterator segment_move = createIterator ( tr->segments );
		TrackSegment *my_segment = nextElement ( &segment_move );

		for ( int i = 0; i < the_length1; i++ ) {

			ListIterator data_waypoint_route = createIterator ( my_segment->waypoints );
			Waypoint *my_waypoint_route = nextElement ( &data_waypoint_route );
			if ( i == 0 ) {
				original_p1y = my_waypoint_route->longitude;
				original_p1x = my_waypoint_route->latitude;
			}

			int the_length2 = my_segment->waypoints->length;

			for ( int i = 1; i < the_length2; i++ ) {

				my_waypoint_route = nextElement( &data_waypoint_route );

				p2y = my_waypoint_route->longitude;
				p2x = my_waypoint_route->latitude;

			}

			if ( i != the_length1-1 ) {

				last_p2y = my_waypoint_route->longitude;
				last_p2x = my_waypoint_route->latitude;

				my_segment = nextElement (&segment_move);

			}

		}

			last_p2y = p2y;
			last_p2x = p2x;

			float dx = 0;
			float dy = 0;
			float dz = 0;

			original_p1y -= last_p2y;
			original_p1y *= (3.1415926536 / 180), original_p1x *= (3.1415926536 / 180), last_p2x *= (3.1415926536 / 180);
		
			dz = sin(original_p1x) - sin(last_p2x);
			dx = cos(original_p1y) * cos(original_p1x) - cos(last_p2x);
			dy = sin(original_p1y) * cos(original_p1x);
			total_dist = total_dist + asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

	}

	total_dist = total_dist * 1000;

	if ( total_dist >= 0 && total_dist <= delta ) {
		return true;
	}

	return false;

}

void tempDelete ( void* data ) {

	return;

}

List* getTracksBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {

	if ( doc == NULL ) {
		return NULL;
	}

	List *betweenTracks;
	betweenTracks = initializeList ( &trackToString, &tempDelete, &compareTracks );

	bool atleastOne = false;

	ListIterator track_iter = createIterator ( doc->tracks );
	Track *my_track = nextElement ( &track_iter );

	float source_dist = 0;
	float dest_dist = 0;

	float original_p1y = 0;
	float original_p1x = 0;
	float last_p2x = 0;
	float last_p2y = 0;

	while ( my_track != NULL ) {

		ListIterator segment_move = createIterator ( my_track->segments );
		TrackSegment *my_segment = nextElement ( &segment_move );

		int the_length1 = my_track->segments->length;

		for ( int i = 0; i < the_length1; i++ ) {

			ListIterator data_waypoint_route = createIterator ( my_segment->waypoints );
			Waypoint *my_waypoint_route = nextElement ( &data_waypoint_route );

			if ( i == 0 ) {
				original_p1y = my_waypoint_route->longitude;
				original_p1x = my_waypoint_route->latitude;
			}
			
			int the_length2 = my_segment->waypoints->length;

			for ( int j = 1; j < the_length2; j++ ) {

				my_waypoint_route = nextElement( &data_waypoint_route );

				last_p2y = my_waypoint_route->longitude;
				last_p2x = my_waypoint_route->latitude;

			}

			if ( i != the_length1-1 ) {

				my_segment = nextElement (&segment_move);

			}

		}

			float dx = 0;
			float dy = 0;
			float dz = 0;

			original_p1y -= sourceLong;
			original_p1y *= (3.1415926536 / 180), original_p1x *= (3.1415926536 / 180), sourceLat *= (3.1415926536 / 180);
		
			dz = sin(original_p1x) - sin(sourceLat);
			dx = cos(original_p1y) * cos(original_p1x) - cos(sourceLat);
			dy = sin(original_p1y) * cos(original_p1x);
			source_dist = asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

			last_p2y -= destLong;
			last_p2y *= (3.1415926536 / 180), last_p2x *= (3.1415926536 / 180), destLat *= (3.1415926536 / 180);
		
			dz = sin(last_p2x) - sin(destLat);
			dx = cos(last_p2y) * cos(last_p2x) - cos(destLat);
			dy = sin(last_p2y) * cos(last_p2x);
			dest_dist = asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

			if ( (source_dist >= 0 && source_dist <= delta) || (dest_dist >= 0 && dest_dist <= delta) ) {
				insertBack ( betweenTracks, (void *)my_track );
				atleastOne = true;
			}

		my_track = nextElement ( &track_iter );

	}

	if ( atleastOne == true ) {
		return betweenTracks;
	}
	else {
		freeList ( betweenTracks );
		return NULL;
	}

	freeList ( betweenTracks );
	return NULL;

}

List* getRoutesBetween ( const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {

	if ( doc == NULL ) {
		return NULL;
	}

	bool atleastOne = false;
	List *betweenRoutes = initializeList ( &routeToString, &tempDelete, &compareRoutes );

	ListIterator route_iter = createIterator ( doc->routes );
	Route *my_route = nextElement ( &route_iter );

	float source_dist, dest_dist;

	float original_p1y, original_p1x, last_p2x, last_p2y;

	while ( my_route != NULL ) {

		ListIterator data_waypoint_route = createIterator ( my_route->waypoints );
		Waypoint *my_waypoint_route = nextElement ( &data_waypoint_route );

		int the_length = my_route->waypoints->length;

		if ( the_length != 0 ) {

			original_p1y = my_waypoint_route->longitude;
			original_p1x = my_waypoint_route->latitude;

			for ( int i = 1; i < the_length; i++ ) {

				my_waypoint_route = nextElement( &data_waypoint_route );

			}

			last_p2y = my_waypoint_route->longitude;
			last_p2x = my_waypoint_route->latitude;

			float dx = 0;
			float dy = 0;
			float dz = 0;

			original_p1y -= sourceLong;
			original_p1y *= (3.1415926536 / 180), original_p1x *= (3.1415926536 / 180), sourceLat *= (3.1415926536 / 180);
		
			dz = sin(original_p1x) - sin(sourceLat);
			dx = cos(original_p1y) * cos(original_p1x) - cos(sourceLat);
			dy = sin(original_p1y) * cos(original_p1x);
			source_dist = asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

			last_p2y -= destLong;
			last_p2y *= (3.1415926536 / 180), last_p2x *= (3.1415926536 / 180), destLat *= (3.1415926536 / 180);
		
			dz = sin(last_p2x) - sin(destLat);
			dx = cos(last_p2y) * cos(last_p2x) - cos(destLat);
			dy = sin(last_p2y) * cos(last_p2x);
			dest_dist = asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

			if ( (source_dist >= 0 && source_dist <= delta) || (dest_dist >= 0 && dest_dist <= delta) ) {
				insertBack ( betweenRoutes, (void *)my_route );
				atleastOne = true;
			}

		}

		my_route = nextElement( &route_iter );

	}

	if ( atleastOne == true ) {
		return betweenRoutes;
	}
	else {
		freeList ( betweenRoutes );
		return NULL;
	}

	freeList ( betweenRoutes );
	return NULL;

}

bool isLoopRoute ( const Route* route, float delta ) {

	if ( route == NULL ) {
		return false;
	}

	if ( delta < 0 ) {
		return false;
	}

	int the_length = route->waypoints->length;

	if ( the_length < 4 ) {
		return false;
	}

	float total_dist = 0;

	float original_p1y = 0;
	float original_p1x = 0;
	float last_p2x = 0;
	float last_p2y = 0;

	if ( route != NULL ) {

		ListIterator data_waypoint_route = createIterator ( route->waypoints );
		Waypoint *my_waypoint_route = nextElement ( &data_waypoint_route );

			original_p1y = my_waypoint_route->longitude;
			original_p1x = my_waypoint_route->latitude;

			for ( int i = 1; i < the_length; i++ ) {

				my_waypoint_route = nextElement( &data_waypoint_route );

			}

			last_p2y = my_waypoint_route->longitude;
			last_p2x = my_waypoint_route->latitude;

			float dx = 0;
			float dy = 0;
			float dz = 0;
			
			original_p1y -= last_p2y;
			original_p1y *= (3.1415926536 / 180), original_p1x *= (3.1415926536 / 180), last_p2x *= (3.1415926536 / 180);
		
			dz = sin(original_p1x) - sin(last_p2x);
			dx = cos(original_p1y) * cos(original_p1x) - cos(last_p2x);
			dy = sin(original_p1y) * cos(original_p1x);
			total_dist = asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

	}

	total_dist = total_dist * 1000;

	if ( total_dist >= 0 && total_dist <= delta ) {
		return true;
	}

	return false;

}

float getRouteLen ( const Route *rt ) {

	if ( rt == NULL ) {
		return 0;
	}

	float total_dist = 0;

	int the_length = rt->waypoints->length;

	if ( rt != NULL ) {

		ListIterator data_waypoint_route = createIterator ( rt->waypoints );
		Waypoint *my_waypoint_route = nextElement ( &data_waypoint_route );

		for ( int i = 1; i < the_length; i++ ) {

			float p1x = 0;
			float p1y = 0;
			float p2x = 0;
			float p2y = 0;

			p1y = my_waypoint_route->longitude;
			p1x = my_waypoint_route->latitude;

			my_waypoint_route = nextElement( &data_waypoint_route );

			p2y = my_waypoint_route->longitude;
			p2x = my_waypoint_route->latitude;

			float dx = 0;
			float dy = 0;
			float dz = 0;
			
			p1y -= p2y;
			p1y *= (3.1415926536 / 180), p1x *= (3.1415926536 / 180), p2x *= (3.1415926536 / 180);
		
			dz = sin(p1x) - sin(p2x);
			dx = cos(p1y) * cos(p1x) - cos(p2x);
			dy = sin(p1y) * cos(p1x);
			total_dist = total_dist + asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * 6371;

		}

	}

	total_dist = total_dist * 1000;

	return total_dist;

}

float round10 ( float len ) {

	int rounded = 0;

  	rounded = ( len + 5 ) / 10;
  	rounded = rounded * 10;
	
	float float_rounded = rounded;

    return float_rounded;

}

xmlDocPtr GPXtoXML ( GPXdoc* doc ) {

    xmlDocPtr XMLdoc = NULL;

    xmlNodePtr root_node = NULL;
	xmlNodePtr node = NULL;
	xmlNodePtr node1 = NULL;
	xmlNodePtr node2 = NULL;
	xmlNsPtr name_s = NULL;
    char temporary[1000];

	/* Retrieval of the following xml function were retrieved from http://knol2share.blogspot.com/2009/05/validate-xml-against-xsd-in-c.html */
    XMLdoc = xmlNewDoc ( BAD_CAST "1.0" );

    root_node = xmlNewNode ( NULL, BAD_CAST "gpx" );
    xmlDocSetRootElement ( XMLdoc, root_node );

	sprintf ( temporary, "%.1f", doc->version );
    xmlNewProp ( root_node, BAD_CAST "version", ( const xmlChar * ) temporary );

    xmlNewProp ( root_node, BAD_CAST "creator", ( const xmlChar * ) doc->creator );

	name_s = xmlNewNs ( root_node, ( const xmlChar * ) doc->namespace, NULL );
	xmlSetNs ( root_node, name_s );

	if ( doc->waypoints != NULL && doc->waypoints->length != 0 ) {

		ListIterator waypoint_iterator = createIterator ( doc->waypoints );
		Waypoint *my_waypoint = nextElement ( &waypoint_iterator );

		ListIterator data_iterator_waypoint = createIterator ( my_waypoint->otherData );
		GPXData *my_data_waypoint = nextElement ( &data_iterator_waypoint );

		while ( my_waypoint != NULL ) {

			node = xmlNewChild ( root_node, NULL, BAD_CAST "wpt", NULL );
			sprintf ( temporary,"%f", my_waypoint->latitude );
			xmlNewProp ( node, BAD_CAST "lat", ( const xmlChar * ) temporary );
			sprintf ( temporary,"%f", my_waypoint->longitude );
			xmlNewProp ( node, BAD_CAST "lon", ( const xmlChar * ) temporary );

			if ( my_waypoint->name != NULL && strcmp ( my_waypoint->name, "" ) != 0 ) {
				node1 = xmlNewChild ( node, NULL, BAD_CAST "name", ( const xmlChar * ) my_waypoint->name );
			}

			if ( my_waypoint->otherData != NULL ) {
				node1 = xmlNewChild ( node, NULL, ( const xmlChar * ) my_data_waypoint->name, ( const xmlChar * ) my_data_waypoint->value );
			}

			my_data_waypoint = nextElement( &data_iterator_waypoint );
			my_waypoint = nextElement( &waypoint_iterator );
		}


	}

	if ( doc->routes != NULL && doc->routes->length != 0 ) {

		ListIterator route_iterator = createIterator ( doc->routes );
		Route *my_route = nextElement ( &route_iterator );

		if ( (my_route->waypoints != NULL && my_route->waypoints->length != 0) ) {

			ListIterator data_iterator_route = createIterator ( my_route->otherData );
			GPXData *my_data_route = nextElement ( &data_iterator_route );

			ListIterator data_waypoint_route = createIterator ( my_route->waypoints );
			Waypoint *my_waypoint_route = nextElement ( &data_waypoint_route );

			ListIterator data_iterator_route_waypoint = createIterator ( my_waypoint_route->otherData );
			GPXData *my_data_route_waypoint = nextElement ( &data_iterator_route_waypoint );

			while ( my_route != NULL && doc->routes->length != 0 ) {

				node = xmlNewChild ( root_node, NULL, BAD_CAST "rte", NULL );

				if ( my_route->name != NULL && strcmp ( my_route->name, "" ) != 0 ) {
					node1 = xmlNewChild ( node, NULL, BAD_CAST "name", ( const xmlChar * ) my_route->name );
				}

				if ( my_route->otherData != NULL ) {
					node1 = xmlNewChild ( node, NULL, ( const xmlChar * ) my_data_route->name, ( const xmlChar * ) my_data_route->value );
				}

				while ( my_waypoint_route != NULL ) {

					node1 = xmlNewChild ( node, NULL, BAD_CAST "rtept", NULL );
					sprintf ( temporary,"%f", my_waypoint_route->latitude );
					xmlNewProp ( node1, BAD_CAST "lat", ( const xmlChar * ) temporary );
					sprintf ( temporary,"%f", my_waypoint_route->longitude );
					xmlNewProp ( node1, BAD_CAST "lon", ( const xmlChar * ) temporary );

					if ( my_waypoint_route->name != NULL && strcmp ( my_waypoint_route->name, "" ) != 0 ) {
						node2 = xmlNewChild ( node1, NULL, BAD_CAST "name", ( const xmlChar * ) my_waypoint_route->name );
					}

					if ( my_waypoint_route->otherData != NULL ) {
						node2 = xmlNewChild ( node1, NULL, ( const xmlChar * ) my_data_route_waypoint->name, ( const xmlChar * ) my_data_route_waypoint->value );
					}

					my_data_route_waypoint = nextElement( &data_iterator_route_waypoint );
					my_waypoint_route = nextElement( &data_waypoint_route );

				}

				my_data_route = nextElement ( &data_iterator_route );
				my_route = nextElement( &route_iterator );

			}

		}

	}

	if ( doc->tracks != NULL && doc->tracks->length != 0 ) {

		ListIterator track_iterator = createIterator ( doc->tracks );

		void *temp = NULL;

		while ( (temp = nextElement(&track_iterator)) != NULL ) {

			Track *my_track = (Track *) temp;

			ListIterator data_iterator_track = createIterator ( my_track->otherData );
			GPXData *my_data_track = nextElement ( &data_iterator_track );

			node = xmlNewChild ( root_node, NULL, BAD_CAST "trk", NULL );

			if ( strcmp ( my_track->name, "" ) != 0 ) {
				node1 = xmlNewChild ( node, NULL, BAD_CAST "name", ( const xmlChar * ) my_track->name );
			}

			if ( my_track->otherData != NULL ) {
				node1 = xmlNewChild ( node, NULL, ( const xmlChar * ) my_data_track->name, ( const xmlChar * ) my_data_track->value );
			}

			void *temp2 = NULL;
			ListIterator data_tracksegment_track = createIterator(my_track->segments);

			while ( (temp2 = nextElement(&data_tracksegment_track)) != NULL ) {

				TrackSegment *my_trackSegment = (TrackSegment *) temp2;

				node1 = xmlNewChild ( node, NULL, BAD_CAST "trkseg", NULL );

				void *temp3 = NULL;
				ListIterator data_waypoint_track = createIterator(my_trackSegment->waypoints);

				while ( (temp3 = nextElement(&data_waypoint_track)) != NULL ) {

					Waypoint *my_waypoint_track = (Waypoint *) temp3;

					ListIterator data_iterator_track_waypoint = createIterator ( my_waypoint_track->otherData );
					GPXData *my_data_track_waypoint = nextElement ( &data_iterator_track_waypoint );

					node2 = xmlNewChild ( node1, NULL, BAD_CAST "trkpt", NULL );
					sprintf ( temporary,"%f", my_waypoint_track->latitude );
					xmlNewProp ( node2, BAD_CAST "lat", ( const xmlChar * ) temporary );
					sprintf ( temporary,"%f", my_waypoint_track->longitude );
					xmlNewProp ( node2, BAD_CAST "lon", ( const xmlChar * ) temporary );

					if ( my_waypoint_track->name != NULL && strcmp ( my_waypoint_track->name, "" ) != 0 ) {
						xmlNewChild ( node2, NULL, BAD_CAST "name", ( const xmlChar * ) my_waypoint_track->name );
					}

					if ( my_waypoint_track->otherData != NULL ) {
						xmlNewChild ( node2, NULL, ( const xmlChar * ) my_data_track_waypoint->name, ( const xmlChar * ) my_data_track_waypoint->value );
					}

				}

			}

		}

	}

	xmlCleanupParser();

	return XMLdoc;

}


bool validator_xml ( xmlDoc *doc , char* gpxSchemaFile) {

	/* Retrieval of the following xml function were retrieved from http://www.xmlsoft.org/examples/tree2.c */
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;

	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewParserCtxt(gpxSchemaFile);
	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);

	if ( doc == NULL ) {
		xmlCleanupParser();
		return false;
	}
	else {

		xmlSchemaValidCtxtPtr ctxt;
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, doc);

		if ( ret == 0 ) {

			xmlSchemaFreeValidCtxt(ctxt);

			if ( schema != NULL ) {
				xmlSchemaFree(schema);
			}

			xmlSchemaCleanupTypes();

			xmlMemoryDump();

			return true;

		}
		else if ( ret > 0 ) {

			xmlSchemaFreeValidCtxt(ctxt);

			if ( schema != NULL ) {
				xmlSchemaFree(schema);
			}

			xmlSchemaCleanupTypes();

			xmlMemoryDump();

			return false;

		}
		else {

			xmlSchemaFreeValidCtxt(ctxt);

			if ( schema != NULL ) {
				xmlSchemaFree(schema);
			}

			xmlSchemaCleanupTypes();

			xmlMemoryDump();

			return false;

		}
	}

	xmlCleanupParser();
	return false;


}

bool writeGPXdoc(GPXdoc* doc, char* fileName) {

	if ( doc == NULL ) {
		return false;
	}

	int k = 0;

	for ( k = 0; fileName[k] != '\0'; k++ ) {}

	if ( !(fileName[k-4] == '.' && fileName[k-3] == 'g'  && fileName[k-2] == 'p'  && fileName[k-1] == 'x') ) {
		return NULL;
	}

	int check = 0;

	xmlDocPtr my_tree = NULL;

	my_tree = GPXtoXML ( doc );

	check = xmlSaveFormatFileEnc ( fileName, my_tree, "UTF-8", 1 );

	xmlFreeDoc ( my_tree );

	if ( check == -1 ) {
		return false;
	}

	return true;

}

bool validateGPXDoc ( GPXdoc* doc, char* gpxSchemaFile ) {

	if ( doc == NULL ) {
		return false;
	}

	bool check = false;

	xmlDocPtr my_tree = NULL;

	my_tree = GPXtoXML ( doc );

	check = validator_xml ( my_tree, gpxSchemaFile );

	xmlFreeDoc ( my_tree );

	if ( check == true ) {
		return true;
	}

	return false;

}

char* trackToJSON ( const Track *tr ) {

	int len;
	float the_length = 0;
	char *tmpStr = NULL;
	char *snum = NULL;

	len = sizeof ( tr ) * sizeof ( Track ) + 100;

	if ( tr == NULL ) {

		tmpStr = (char*) malloc ( sizeof(char)*len );

		strcpy ( tmpStr, "[]" );
		return tmpStr;
	}

	len = sizeof( char * ) * (sizeof( tr->otherData ) * sizeof( tr->segments ) * sizeof ( tr ) * sizeof ( Track ) ) + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );
	snum = (char*) malloc ( sizeof(char)*len );



	strcpy ( tmpStr, "{\"name\":\"" );
	if ( tr->name == NULL || strcmp ( tr->name, "" ) == 0 ) {
		strcat ( tmpStr, "" );
	} else {
		strcat ( tmpStr, tr->name );
	}
	strcat ( tmpStr, "\",\"len\":" );
	the_length = round10 ( getTrackLen ( tr ) );
	sprintf ( snum, "%.1f", the_length );
	strcat ( tmpStr, snum );
	strcat ( tmpStr, ",\"loop\":" );
	if ( (isLoopTrack ( tr, 10 )) == true ) {
		strcat ( tmpStr, "true" );
	} else {
		strcat ( tmpStr, "false" );
	}
	strcat ( tmpStr, "}" );

	free ( snum );

	return tmpStr;

}

char* routeToJSON ( const Route *rt ) {

	int len = 0;
	float the_length = 0;
	char *snum = NULL;
	char *tmpStr = NULL;
	int num_routes = 0;

	len = sizeof ( Route ) * sizeof ( rt ) + 100;

	if ( rt == NULL ) {

		tmpStr = (char*) malloc ( sizeof(char)*len );

		strcpy ( tmpStr, "[]" );
		return tmpStr;
	}

	len = sizeof( char * ) * (sizeof ( rt->waypoints ) * sizeof( rt->otherData ) * sizeof ( Route ) * sizeof ( rt ) ) + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );
	snum = (char*) malloc ( sizeof(char)*len );

	strcpy ( tmpStr, "{\"name\":\"" );
	if ( rt->name == NULL || strcmp ( rt->name, "" ) == 0 ) {
		strcat ( tmpStr, "" );
	} else {
		strcat ( tmpStr, rt->name );
	}
	strcat ( tmpStr, "\",\"numPoints\":" );
	num_routes = getLength ( rt->waypoints );
	sprintf ( snum, "%d", num_routes );
	strcat ( tmpStr, snum );
	strcat ( tmpStr, ",\"len\":" );
	the_length = round10 ( getRouteLen ( rt ) );
	sprintf ( snum, "%.1f", the_length );
	strcat ( tmpStr, snum );
	strcat ( tmpStr, ",\"loop\":" );
	if ( (isLoopRoute ( rt, 10 )) == true ) {
		strcat ( tmpStr, "true" );
	} else {
		strcat ( tmpStr, "false" );
	}
	strcat ( tmpStr, "}" );

	free ( snum );

	return tmpStr;

}

void addWaypoint(Route *rt, Waypoint *pt) {

	if ( rt == NULL || pt == NULL ) {
		return;
	}

	insertBack ( rt->waypoints, (void *) pt );

	return;

}

void addRoute(GPXdoc* doc, Route* rt) {

	if ( doc == NULL || rt == NULL ) {
		return;
	}

	insertBack ( doc->routes, (void *) rt );

	return;

}

Route* JSONtoRoute ( const char* gpxString ) {

	if ( gpxString == NULL ) {
		return NULL;
	}

	int len = 0;
	char *my_name = NULL;
	int i = 0;
	int j = 0;

	Route *my_route = ( Route *) malloc ( sizeof ( Route ) );
	my_route->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );
	my_route->waypoints = initializeList ( &waypointToString, &deleteWaypoint, &compareWaypoints );

	// Waypoint *my_waypoint = ( Waypoint *) malloc ( sizeof ( Waypoint ) );
	// my_waypoint->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

	// len = strlen ( gpxString );
	// my_waypoint->name = (char *) ( malloc ( ( len ) + 1 ) );
	// my_waypoint->name[0] = '\0';

	len = strlen ( gpxString );

	my_name = (char *) ( malloc ( ( len ) + 1 ) );
	my_route->name = (char *) ( malloc ( ( len ) + 1 ) );

	for ( i = 9; gpxString[i] != '\"'; i++ ) {
		my_name[j] = gpxString[i];
		j++;
	}

	my_name[j] = '\0';

	strcpy ( my_route->name, (char *)( my_name ) );

	free ( my_name );

	return my_route;

}

Waypoint* JSONtoWaypoint ( const char* gpxString ) {

	if ( gpxString == NULL ) {
		return NULL;
	}

	char *my_lat = NULL;
	char *my_lon = NULL;;

	Waypoint *my_waypoint = ( Waypoint *) malloc ( sizeof ( Waypoint ) );

	int len = strlen ( gpxString );
	my_waypoint->name = (char *) ( malloc ( ( len ) + 1 ) );
	my_waypoint->name[0] = '\0';

	my_lat = (char *) ( malloc ( ( len ) + 1 ) );
	my_lon = (char *) ( malloc ( ( len ) + 1 ) );

	my_waypoint->otherData = initializeList ( &gpxDataToString, &deleteGpxData, &compareGpxData );

	int i = 0;
	int j = 0;
	int k = 0;

	for ( i = 7; gpxString[i] != ','; i++ ) {
		my_lat[j] = gpxString[i];
		j++;
	}

	my_lat[j] = '\0';

	my_waypoint->latitude = atof ( my_lat );

	for ( i = 14+j; gpxString[i] != '}'; i++ ) {
		my_lon[k] = gpxString[i];
		k++;
	}

	my_lon[k] = '\0';

	my_waypoint->longitude = atof ( my_lon );

	free ( my_lat );
	free ( my_lon );

	return my_waypoint;

}

GPXdoc* JSONtoGPX(const char* gpxString) {

	if ( gpxString == NULL ) {
		return NULL;
	}

	int i = 0;
	int j = 0;
	int k = 0;

	char *my_version = NULL;
	char *my_creator = NULL;

	GPXdoc *my_doc = (GPXdoc *) malloc ( sizeof ( GPXdoc ) );

	int len = strlen ( gpxString );
	my_doc->creator = (char *) ( malloc ( ( len ) + 1 ) );
	my_version = (char *) ( malloc ( ( len ) + 1 ) );
	my_creator = (char *) ( malloc ( ( len ) + 1 ) );

	my_doc->waypoints = initializeList ( &waypointToString, &deleteWaypoint, &compareWaypoints );
	my_doc->routes = initializeList ( &routeToString, &deleteRoute, &compareRoutes );
	my_doc->tracks = initializeList ( &trackToString, &deleteTrack, &compareTracks );

	for ( i = 11; gpxString[i] != ','; i++ ) {
		my_version[j] = gpxString[i];
		j++;
	}

	my_version[j] = '\0';

	my_doc->version = atof ( my_version );

	for ( i = 23+j; gpxString[i] != '\"'; i++ ) {
		my_creator[k] = gpxString[i];
		k++;
	}

	my_creator[k] = '\0';

	strcpy ( my_doc->creator, (char *)( my_creator ) );
	strcpy ( my_doc->namespace, (char *)( "http://www.topografix.com/GPX/1/1" ) );

	free ( my_version );
	free ( my_creator );

	return my_doc;

}


char* GPXtoJSON ( const GPXdoc* gpx ) {

	int len;
	int num = 0;
	char *snum = NULL;
	char *tmpStr = NULL;

	len = sizeof ( GPXdoc ) * sizeof ( int ) * sizeof ( int ) * sizeof ( int ) * sizeof ( float ) + 100;

	if ( gpx == NULL ) {

		tmpStr = (char*) malloc ( sizeof(char)*len );

		strcpy ( tmpStr, "[]" );
		return tmpStr;
	}

	len = strlen( gpx->creator ) + sizeof ( GPXdoc ) * sizeof ( int ) * sizeof ( int ) * sizeof ( int ) * sizeof ( float ) + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );
	snum = (char*) malloc ( sizeof(char)*len );

	strcpy ( tmpStr, "{\"version\":" );
	sprintf ( snum, "%.1f", gpx->version );
	strcat ( tmpStr, snum );
	strcat ( tmpStr, ",\"creator\":\"" );
	strcat ( tmpStr, gpx->creator );
	strcat ( tmpStr, "\",\"numWaypoints\":" );
	num = getNumWaypoints ( gpx );
	sprintf ( snum, "%d", num );
	strcat ( tmpStr, snum );
	strcat ( tmpStr, ",\"numRoutes\":" );
	num = getNumRoutes ( gpx );
	sprintf ( snum, "%d", num );
	strcat ( tmpStr, snum );
	strcat ( tmpStr, ",\"numTracks\":" );
	num = getNumTracks ( gpx );
	sprintf ( snum, "%d", num );
	strcat ( tmpStr, snum );
	strcat ( tmpStr, "}" );

	free ( snum );

	return tmpStr;

}

char* trackListToJSON ( const List *list ) {

	int len;
	char *tmpStr = NULL;

	len = sizeof ( Track ) * sizeof ( list ) * sizeof ( char * ) + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );

	if ( list == NULL || list->length == 1 ) {
		strcpy ( tmpStr, "[]" );
		return tmpStr;
	}

	ListIterator track_iter = createIterator ( (List *)list );
	Track *my_track = nextElement ( &track_iter );

	strcpy ( tmpStr, "[" );

	while ( my_track != NULL ) {

		char *tmpStr2 = trackToJSON ( my_track );

		strcat ( tmpStr, tmpStr2 );

		my_track = nextElement ( &track_iter );

		if ( my_track != NULL ) {
			strcat ( tmpStr, ",");
		}

		free ( tmpStr2 );

	}

	strcat ( tmpStr, "]");

	return tmpStr;

}

char* routeListToJSON ( const List *list ) {

	int len;
	char *tmpStr = NULL;

	len = sizeof ( Route ) * sizeof ( list ) * sizeof ( char * ) + 100;
	tmpStr = (char*) malloc ( sizeof(char)*len );

	if ( list == NULL || list->length == 0 ) {
		strcpy ( tmpStr, "[]" );
		return tmpStr;
	}

	ListIterator route_iter = createIterator ( (List *)list );
	Route *my_route = nextElement ( &route_iter );

	strcpy ( tmpStr, "[" );

	while ( my_route != NULL ) {

		char *tmpStr2 = routeToJSON ( my_route );

		strcat ( tmpStr, tmpStr2 );

		my_route = nextElement ( &route_iter );

		if ( my_route != NULL ) {
			strcat ( tmpStr, ",");
		}

		free ( tmpStr2 );

	}

	strcat ( tmpStr, "]");

	return tmpStr;

}

GPXdoc* createValidGPXdoc ( char* fileName, char* gpxSchemaFile) {

	if ( fileName == NULL || strcmp ( fileName, "" ) == 0 ) {
        fprintf ( stderr, "File name cannot be an empty string or NULL.\n" );
		return NULL;
    }

	if ( gpxSchemaFile == NULL || strcmp ( gpxSchemaFile, "" ) == 0 ) {
        fprintf ( stderr, "Schema File cannot be an empty string or NULL.\n" );
		return NULL;
    }

	int k = 0;
	int h = 0;

	for ( k = 0; fileName[k] != '\0'; k++ ) {}
	for ( h = 0; gpxSchemaFile[h] != '\0'; h++ ) {}

	if ( !(fileName[k-4] == '.' && fileName[k-3] == 'g'  && fileName[k-2] == 'p'  && fileName[k-1] == 'x') ) {
		return NULL;
	}

	if ( !(gpxSchemaFile[h-4] == '.' && gpxSchemaFile[h-3] == 'x'  && gpxSchemaFile[h-2] == 's'  && gpxSchemaFile[h-1] == 'd') ) {
		return NULL;
	}

    /*
     * Code from lines 10-29 were retrieved from the libXmlExample.c program
     * provided by Professor Nikitenko via CourseLink
     */
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    xmlNode *cur_node = NULL;

    /*
     * This initializes the library and checks for potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* Parse the file and get the DOM, retrieved from http://xmlsoft.org/ */
    doc = xmlReadFile ( fileName, NULL, 0 );

    if ( doc == NULL ) {
        fprintf ( stderr, "Failed to parse %s\n", fileName );
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return NULL;
    }
	else {
		if ( validator_xml(doc, gpxSchemaFile) == false ) {
			xmlFreeDoc(doc);
			xmlCleanupParser();
			return NULL;
		}
	}

    /* Get the root element node, retrieved from http://xmlsoft.org/ */
    root_element = xmlDocGetRootElement(doc);

	GPXdoc *my_doc = (GPXdoc *) malloc ( sizeof ( GPXdoc ) );

	/* Initialize Lists */
	my_doc->waypoints = initializeList ( &waypointToString, &deleteWaypoint, &compareWaypoints );
	my_doc->routes = initializeList ( &routeToString, &deleteRoute, &compareRoutes );
	my_doc->tracks = initializeList ( &trackToString, &deleteTrack, &compareTracks );

    for (cur_node = root_element; cur_node != NULL; cur_node = cur_node->next) {

        for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next) {

            xmlNode *temp_node = cur_node;

			/* Check file for Waypoints */
            if ( strcmp ( "wpt", (char *)(cur_node->name) ) == 0 ) {

				insertBack ( my_doc->waypoints, waypoint_function ( temp_node ) );

			}

			/* Check file for Routes */
            if ( strcmp ( "rte", (char *)(cur_node->name) ) == 0 ) {

				insertBack ( my_doc->routes, route_function ( temp_node ) );

            }

			/* Check file for Tracks */
			if ( strcmp ( "trk", (char *)(cur_node->name) ) == 0 ) {

				insertBack ( my_doc->tracks, track_function ( temp_node ) );

            }

        }

        cur_node = root_element;

        xmlAttr *attr;
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);

			/* Retrieval of namespace was retrieved from http://xmlsoft.org/ */
			strcpy ( my_doc->namespace, (char *)( cur_node->ns->href ) );

            if ( strcmp ( "version", attrName ) == 0 ) {
                double parsed_version = strtod ( cont, NULL );
                my_doc->version = parsed_version;
            }

            else if ( strcmp ( "creator", attrName ) == 0 ) {
				int len = strlen ( cont );
				my_doc->creator = (char *) ( malloc ( ( len ) + 1 ) );
				strcpy ( my_doc->creator, (char *)( value->content ) );
            }

        }

    }

	/* xmlFreeDoc function was retrieved from http://xmlsoft.org/ */
	xmlFreeDoc(doc);
	xmlCleanupParser();

    return my_doc;

}

GPXdoc* createGPXdoc ( char* fileName ) {

    if ( fileName == NULL || strcmp ( fileName, "" ) == 0 ) {
        fprintf ( stderr, "File name cannot be an empty string or NULL.\n" );
		return NULL;
    }

    GPXdoc *my_doc = (GPXdoc *) malloc ( sizeof ( GPXdoc ) );

    /*
     * Code from lines 10-29 were retrieved from the libXmlExample.c program
     * provided by Professor Nikitenko via CourseLink
     */
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    xmlNode *cur_node = NULL;

    /*
     * This initializes the library and checks for potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* Parse the file and get the DOM, retrieved from http://xmlsoft.org/ */
    doc = xmlReadFile ( fileName, NULL, 0 );

    if ( doc == NULL ) {
        fprintf ( stderr, "Failed to parse %s\n", fileName );
		return NULL;
    }

    /* Get the root element node, retrieved from http://xmlsoft.org/ */
    root_element = xmlDocGetRootElement(doc);

	/* Initialize Lists */
	my_doc->waypoints = initializeList ( &waypointToString, &deleteWaypoint, &compareWaypoints );
	my_doc->routes = initializeList ( &routeToString, &deleteRoute, &compareRoutes );
	my_doc->tracks = initializeList ( &trackToString, &deleteTrack, &compareTracks );

    for (cur_node = root_element; cur_node != NULL; cur_node = cur_node->next) {

        for (cur_node = cur_node->children; cur_node != NULL; cur_node = cur_node->next) {

            xmlNode *temp_node = cur_node;

			/* Check file for Waypoints */
            if ( strcmp ( "wpt", (char *)(cur_node->name) ) == 0 ) {

				insertBack ( my_doc->waypoints, waypoint_function ( temp_node ) );

			}

			/* Check file for Routes */
            if ( strcmp ( "rte", (char *)(cur_node->name) ) == 0 ) {

				insertBack ( my_doc->routes, route_function ( temp_node ) );

            }

			/* Check file for Tracks */
			if ( strcmp ( "trk", (char *)(cur_node->name) ) == 0 ) {

				insertBack ( my_doc->tracks, track_function ( temp_node ) );

            }

        }

        cur_node = root_element;

        xmlAttr *attr;
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);

			/* Retrieval of namespace was retrieved from http://xmlsoft.org/ */
			strcpy ( my_doc->namespace, (char *)( cur_node->ns->href ) );

            if ( strcmp ( "version", attrName ) == 0 ) {
                double parsed_version = strtod ( cont, NULL );
                my_doc->version = parsed_version;
            }

            else if ( strcmp ( "creator", attrName ) == 0 ) {
				int len = strlen ( cont );
				my_doc->creator = (char *) ( malloc ( ( len ) + 1 ) );
				strcpy ( my_doc->creator, (char *)( value->content ) );
            }

        }

    }

	/* xmlFreeDoc function was retrieved from http://xmlsoft.org/ */
	xmlFreeDoc(doc);

    return my_doc;

}

char* GPXdocToString ( GPXdoc* doc ) {

	char *waypoint_return = toString ( doc->waypoints );
	char *route_return = toString ( doc->routes );
	char *track_return = toString ( doc->tracks );

	int len_doc = ((sizeof ( doc->version ) + strlen ( doc->creator ) + strlen ( doc->namespace ) + strlen ( waypoint_return ) + strlen ( route_return ) + strlen ( track_return )) * sizeof ( GPXdoc )) + 100;

    char *human_string = (char *) malloc ( sizeof ( char ) * len_doc );

	sprintf ( human_string, "\nThe version is: %f\nThe creator is: %s\nThe namespace is: %s\n%s%s%s\n", doc->version, doc->creator, doc->namespace, waypoint_return, route_return, track_return );

	free ( waypoint_return );
	free ( route_return );
	free ( track_return );

    return ( human_string );

}

void deleteGPXdoc ( GPXdoc* doc ) {

	if ( doc == NULL ) {
		return;
    }

    free ( doc->creator );

	freeList ( doc->waypoints );
	freeList ( doc->routes );
	freeList ( doc->tracks );

    free ( doc );

}
