#include <string>
#include <iostream>

#if !defined( WIN32 )
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE Minimal

#include <boost/test/unit_test.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "voronoidiagram.hpp"
#include "version.hpp"

#include "utility/vd2svg.hpp"

bool pointInTriangle(ovd::Point p, ovd::Point p0, ovd::Point p1, ovd::Point p2)
{
    double doubleArea = -p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;
    double sign = doubleArea < 0 ? -1 : 1;
    double s = (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y) * sign;
    double t = (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y) * sign;
    return s > 0 && t > 0 && (s + t) < doubleArea * sign;
}

std::ostream& operator<<(std::ostream& lhs, ovd::VertexType e) {
    switch(e) {
        case ovd::OUTER: lhs << "OUTER"; break;
        case ovd::NORMAL: lhs << "NORMAL"; break;
        case ovd::POINTSITE: lhs << "POINTSITE"; break;
        case ovd::ENDPOINT: lhs << "ENDPOINT"; break;
        case ovd::SEPPOINT: lhs << "SEPPOINT"; break;
        case ovd::APEX: lhs << "APEX"; break;
        case ovd::SPLIT: lhs << "SPLIT"; break;
        default :
            lhs << e; break;
    }
    return lhs;
}

std::map<ovd::VertexType, std::vector<ovd::HEVertex> > getVertices(ovd::HEGraph& g) {
    std::map<ovd::VertexType, std::vector<ovd::HEVertex> > m;
    BOOST_FOREACH( ovd::HEVertex v, g.vertices() ) {
        std::cout << g[v].type << g[v].position << "|" << boost::degree(v, g.g) << std::endl;
        m[g[v].type].push_back(v);
    }
    return m;
}

// very simple OpenVoronoi example program
BOOST_AUTO_TEST_CASE( my_test )
{
    ovd::VoronoiDiagram* vd = new ovd::VoronoiDiagram(1,100); // (r, bins)
    // double r: radius of circle within which all input geometry must fall. use 1 (unit-circle). Scale geometry if necessary.
    // int bins: bins for face-grid search. roughly sqrt(n), where n is the number of sites is good according to Held.
     
    std::cout << "OpenVoronoi version " << ovd::version() << "\n"; // the git revision-string
    std::cout << "build-type " <<ovd::build_type() << "\n"; // the build tybe (Release, Debug, Profile, etc)
    std::cout << "compiler " <<ovd::compiler() << "\n";
    std::cout << "system " <<ovd::system() << "\n";
    std::cout << "processor " <<ovd::processor() << "\n";
    
    std::cout << vd->print();
    vd2svg("empty.svg", vd);
    BOOST_CHECK_EQUAL(vd->num_faces(), 3);
    
    ovd::HEGraph& g = vd->get_graph_reference();
    std::map<ovd::VertexType, std::vector<ovd::HEVertex> > vertices = getVertices(g);
    std::vector<ovd::HEVertex> pointSites = vertices[ovd::POINTSITE];
    //check that we found the initial triangle
    BOOST_CHECK_EQUAL(pointSites.size(), 3);
    ovd::Point p0 = g[pointSites[0]].position,
        p1 = g[pointSites[1]].position,
        p2 = g[pointSites[2]].position;
    //check that the unit square fits inside the initial triangle
    BOOST_CHECK(pointInTriangle(ovd::Point(-1, -1), p0, p1, p2));
    BOOST_CHECK(pointInTriangle(ovd::Point(-1, 1), p0, p1, p2));
    BOOST_CHECK(pointInTriangle(ovd::Point(1, -1), p0, p1, p2));
    BOOST_CHECK(pointInTriangle(ovd::Point(1, 1), p0, p1, p2));
    
    BOOST_CHECK_EQUAL(vertices[ovd::NORMAL].size(), 1);
    BOOST_CHECK_EQUAL(g[vertices[ovd::NORMAL][0]].position, ovd::Point(0, 0));
    
    std::cout << "____\nInserting one point at (0,0)";
    ovd::Point p(0,0);
    vd->insert_point_site(p); // this returns an int-handle to the point-site, but we do not use it here.
    
    std::cout << vd->print();
    vd2svg("minimal.svg", vd);
    vertices = getVertices(g);
    BOOST_CHECK_EQUAL(vd->num_faces(), 4);
    BOOST_CHECK_EQUAL(vertices[ovd::NORMAL].size(), 3);
    delete vd;
}
