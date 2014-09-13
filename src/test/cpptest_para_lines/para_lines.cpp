
// OpenVoronoi polygon example

#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#if !defined( WIN32 )
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE para_lines

#include <boost/test/unit_test.hpp>

#include "voronoidiagram.hpp"
#include "version.hpp"
#include "common/point.hpp"
#include "utility/vd2svg.hpp"

#include <boost/random.hpp>
#include <boost/timer.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

void test_para(bool reverse) {
    ovd::VoronoiDiagram vd(1,10);
    std::cout << "OpenVoronoi version: " << ovd::version() << "\n";
    
    std::vector<int> vertex_ids;
    std::vector<ovd::Point> vertices;
    vertices.push_back( ovd::Point(   0, 0) );
    vertices.push_back( ovd::Point( 0.1, 0.1) );
    vertices.push_back( ovd::Point( 0.2, 0.2) );
    
    // point-sites must be inserted first.
    // insert_point_site() returns an int-handle that is used when inserting line-segments
    BOOST_FOREACH(ovd::Point p, vertices ) {
        vertex_ids.push_back( vd.insert_point_site(p) );
    }
    
    // now we insert line-segments
    if (reverse) {
        vd.insert_line_site( vertex_ids[0], vertex_ids[1]);
        vd.insert_line_site( vertex_ids[2], vertex_ids[1]);
    } else {
        vd.insert_line_site( vertex_ids[0], vertex_ids[1]);
        vd.insert_line_site( vertex_ids[1], vertex_ids[2]);
    }
    
    std::cout << " Correctness-check: " << vd.check() << "\n";
    std::cout << vd.print();
    vd2svg("polygon.svg", &vd);
}


BOOST_AUTO_TEST_CASE( test_para_lines ) {
    test_para(false);
}

BOOST_AUTO_TEST_CASE( test_para_lines_r ) {
    test_para(true);
}
