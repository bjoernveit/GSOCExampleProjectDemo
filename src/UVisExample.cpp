#include "ros/ros.h"
#include "unreal_vis_msgs/DisplayBasicMarker.h"
#include "unreal_vis_msgs/DisplayTrajectory.h"
#include "unreal_vis_msgs/RemoveMarker.h"
#include <cstdlib>



void TestDisplayBasicMarker(ros::NodeHandle n, int px, int py, int pz, double scale, int type, double alpha)
{
    ros::ServiceClient client = n.serviceClient<unreal_vis_msgs::DisplayBasicMarker>("unreal/display_basic_marker");

    unreal_vis_msgs::DisplayBasicMarker srv;

    geometry_msgs::Pose pose;
	pose.position.x = px;
	pose.position.y = py;
	pose.position.z = pz;

	pose.orientation.x = 0;
	pose.orientation.y = 0;
	pose.orientation.z = 0;
	pose.orientation.w = 1;
    srv.request.marker_type = type % 6;
    srv.request.pose = pose;
    srv.request.color.r = (type % 6) * 50.0;
    srv.request.color.g = 255.0 - (type % 6) * 50.0;
    srv.request.color.b = (double)((120 + (type % 6) * 50) % 255);
    srv.request.color.a = alpha;
    srv.request.scale = scale;
    switch((int)type % 6){
    case 0:
        srv.request.marker_id = "BasicMarkerPoint";
        break;
    case 1:
        srv.request.marker_id = "BasicMarkerCube";
        break;
    case 2:
        srv.request.marker_id = "BasicMarkerSphere";
        break;
    case 3:
        srv.request.marker_id = "BasicMarkerCylinder";
        break;
    case 4:
        srv.request.marker_id = "BasicMarkerCone";
        break;
    case 5:
        srv.request.marker_id = "BasicMarkerArrow";
        break;
    default:
        srv.request.marker_id = "BasicMarker";
        break;
    }

    if(client.call(srv)){
        ROS_INFO("'DisplayBasicMarker' was called!");
    } else {
        ROS_ERROR("Failed to call service 'DisplayBasicMarker'");
    }
}




void DisplayTrajectory(ros::NodeHandle n, double start_x, double start_y, double start_z, double end_x, double end_y, double end_z)
{
    ros::ServiceClient client = n.serviceClient<unreal_vis_msgs::DisplayTrajectory>("unreal/display_trajectory");

    unreal_vis_msgs::DisplayTrajectory srv;
    int number_of_points = 1000;

    for (int i = 0; i < number_of_points; ++i)
    {
        geometry_msgs::Point point;
        point.x = start_x + ((i/(double)number_of_points) * (end_x - start_x));
        point.y = start_y + ((i/(double)number_of_points) * (end_y - start_y));
        point.z = start_z + ((i/(double)number_of_points) * (end_z - start_z));
        srv.request.points.push_back(point);
    }

    srv.request.color_begin.r = 0.0;
    srv.request.color_begin.g = 255.0;
    srv.request.color_begin.b = 0;

    srv.request.color_end.r = 255.0;
    srv.request.color_end.g = 0.0;
    srv.request.color_end.b = 0.0;

    srv.request.marker_id = "TrajectoryMarker";

    if(client.call(srv)){
        ROS_INFO("'DisplayTrajectory' was called!");

    } else {
        ROS_ERROR("Failed to call service 'DisplayTrajectory'");
    }
}

void TestRemoveMarker(ros::NodeHandle n, std::string id)
{
    ros::ServiceClient client = n.serviceClient<unreal_vis_msgs::RemoveMarker>("unreal/remove_marker");

    unreal_vis_msgs::RemoveMarker srv;
    srv.request.marker_id = id;
    if(client.call(srv)){
        ROS_INFO("'RemoveMarker' was called!");
    } else {
        ROS_ERROR("Failed to call service 'RemoveMarker'");
    }
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "UVisPackage_test_client");
    ros::NodeHandle n;


    // Trajectorie testing
    int cordinate_range = 1000;
    int number_of_trajectories = 50;


    for(int i = 0; i<number_of_trajectories; i++)
    {
        double sx = (double)((std::rand()%cordinate_range) - cordinate_range/2);
        double sy = (double)((std::rand()%cordinate_range) - cordinate_range/2);
        double sz = (double)((std::rand()%cordinate_range));
        double ex = (double)((std::rand()%cordinate_range) - cordinate_range/2);
        double ey = (double)((std::rand()%cordinate_range) - cordinate_range/2);
        double ez = (double)((std::rand()%cordinate_range));
        DisplayTrajectory(n, sx, sy, sz, ex, ey, ez);
    }
    // wait for user to hit enter
    ROS_INFO("Hit enter to remove Trajectories and try spawing makers instead.");
    std::cin.ignore();
    TestRemoveMarker(n, "TrajectoryMarker");


    // Marker testing

//void TestDisplayBasicMarker(ros::NodeHandle n, int px, int py, int pz, double scale, int type)

    // for all marker types five sizes in rows
    for(int t = 0; t<6; ++t)
    {
        int py = -600 + t*200;
        for(int s = 1; s < 6; ++s)
        {
            int px = -600 + s * 200;
            double scale = 1 + (double)s/5;
            double alpha = 155 + (double)20 * s;
            TestDisplayBasicMarker(n, px, py, 100, scale, t, alpha);
        }
    }


    // wait for user to hit enter
    ROS_INFO("Hit enter to remove Markers");
    std::cin.ignore();


    TestRemoveMarker(n, "BasicMarkerPoint");
    TestRemoveMarker(n, "BasicMarkerCube");
    TestRemoveMarker(n, "BasicMarkerSphere");
    TestRemoveMarker(n, "BasicMarkerCylinder");
    TestRemoveMarker(n, "BasicMarkerCone");
    TestRemoveMarker(n, "BasicMarkerArrow");
}
