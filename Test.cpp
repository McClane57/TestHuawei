#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>

#include "tPoint.h"
#include "tOctet.h"
#include "limits.h"
#include "tSegment.h"



BOOST_AUTO_TEST_SUITE(tPointTests)

BOOST_AUTO_TEST_CASE(Constructor_test)
{
	tPoint point0;
	BOOST_CHECK(point0.x == 0);
	BOOST_CHECK(point0.y == 0);

	tPoint point1(10, 20);
	BOOST_CHECK(point1.x == 10);
	BOOST_CHECK(point1.y == 20);
}

BOOST_AUTO_TEST_CASE(Operators_test)
{
	tPoint point0;
	tPoint point1(10, 20);

	tPoint sum;
	sum = point0 + point1;
	BOOST_CHECK(sum.x == 10);
	BOOST_CHECK(sum.y == 20);

	tPoint diff;
	diff = point0 - point1;
	BOOST_CHECK(diff.x == -10);
	BOOST_CHECK(diff.y == -20);
	
	tPoint mult;
	mult = point1 * (-13);
	BOOST_CHECK(mult.x == -130);
	BOOST_CHECK(mult.y == -260);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Limits_test)

BOOST_AUTO_TEST_CASE(Directions_test)
{
	eRegDir dir1 = eRegDir::SE;
	BOOST_CHECK(Next(dir1) == 0);
	BOOST_CHECK(Next(Next(dir1), 1, false) == 7);

	eRegDir dir2 = eRegDir::NW;
	BOOST_CHECK(Next(dir2) == 4);
	BOOST_CHECK(Next(Next(dir2),1, false) == 3);

	BOOST_CHECK(OrthogonalDirection(eRegDir::NE) == eRegDir::NW);
	BOOST_CHECK(OrthogonalDirection(eRegDir::NE, false) == eRegDir::SE);
	BOOST_CHECK(OppositeDirection(eRegDir::SW) == eRegDir::NE);

	BOOST_CHECK(NegativeDirection(eRegDir::NE) == eRegDir::SE);
}

BOOST_AUTO_TEST_CASE(point_limits)
{
	tPoint point(10, 4);

	BOOST_CHECK(point.Limit(E) == 10);
	BOOST_CHECK(point.Limit(NE) == 14);
	BOOST_CHECK(point.Limit(N) == 4);
	BOOST_CHECK(point.Limit(NW) == -6);
	BOOST_CHECK(point.Limit(W) == -10);
	BOOST_CHECK(point.Limit(SW) == -14);
	BOOST_CHECK(point.Limit(S) == -4);
	BOOST_CHECK(point.Limit(SE) == 6);
}

BOOST_AUTO_TEST_CASE(segment_limits)
{
	tPoint start(10, 4);
	tPoint finish(1, 1);
	tSegment segment(start, finish);

	BOOST_CHECK(segment.Limit(E) == 10);
	BOOST_CHECK(segment.Limit(NE) == 14);
	BOOST_CHECK(segment.Limit(N) == 4);
	BOOST_CHECK(segment.Limit(NW) == 0);
	BOOST_CHECK(segment.Limit(W) == -1);
	BOOST_CHECK(segment.Limit(SW) == -2);
	BOOST_CHECK(segment.Limit(S) == -1);
	BOOST_CHECK(segment.Limit(SE) == 6);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tSegment_test)

BOOST_AUTO_TEST_CASE(regularity_test)
{
	tPoint start1(20, -30);
	tPoint finish1(50, -60);

	tSegment segment1(start1, finish1);
	BOOST_CHECK(segment1.IsRegular() == true);

	tPoint start2(20, -30);
	tPoint finish2(50, -30);

	tSegment segment2(start2, finish2);
	BOOST_CHECK(segment2.IsRegular() == true);

	tPoint start3(20, -30);
	tPoint finish3(50, -20);

	tSegment segment3(start3, finish3);
	BOOST_CHECK(segment3.IsRegular() == false);
}

BOOST_AUTO_TEST_CASE(directions_test)
{
	tPoint start1(20, -30);
	tPoint finish1(50, -60);

	tSegment segment1(start1, finish1);
	BOOST_CHECK(segment1.Directions().first == SE);
	BOOST_CHECK(segment1.Directions().second == SE);


	tPoint start2(20, -30);
	tPoint finish2(50, -30);

	tSegment segment2(start2, finish2);
	BOOST_CHECK(segment2.Directions().first == E);
	BOOST_CHECK(segment2.Directions().second == E);

	tPoint start3(20, -30);
	tPoint finish3(50, -20);

	tSegment segment3(start3, finish3);
	BOOST_CHECK(segment3.Directions().first == E);
	BOOST_CHECK(segment3.Directions().second == NE);

	tPoint start4(20, -30);
	tPoint finish4(50, -40);

	tSegment segment4(start4, finish4);
	BOOST_CHECK(segment4.Directions().first == SE);
	BOOST_CHECK(segment4.Directions().second == E);

	tPoint start5(20, -30);
	tPoint finish5(0, -40);

	tSegment segment5(start5, finish5);
	BOOST_CHECK(segment5.Directions().first == W);
	BOOST_CHECK(segment5.Directions().second == SW);

	tPoint start6(20, -30);
	tPoint finish6(0, 0);


	tSegment segment6(start6, finish6);
	BOOST_CHECK(segment6.Directions().first == NW);
	BOOST_CHECK(segment6.Directions().second == N);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Octet_tests)

BOOST_AUTO_TEST_CASE(constructor_test)
{
	tOctet octet1;
	for (auto i = 0; i < 8; ++i) {
		BOOST_CHECK(octet1.Limit(eRegDir(i)) == 0);
	}

	tPoint point(10, 4);
	tOctet octet2(point);

	BOOST_CHECK(octet2.Limit(E) == 10);
	BOOST_CHECK(octet2.Limit(NE) == 14);
	BOOST_CHECK(octet2.Limit(N) == 4);
	BOOST_CHECK(octet2.Limit(NW) == -6);
	BOOST_CHECK(octet2.Limit(W) == -10);
	BOOST_CHECK(octet2.Limit(SW) == -14);
	BOOST_CHECK(octet2.Limit(S) == -4);
	BOOST_CHECK(octet2.Limit(SE) == 6);

	tPoint start(10, 4);
	tPoint finish(1, 1);
	tSegment segment(start, finish);
	tOctet octet3(segment);

	BOOST_CHECK(octet3.Limit(E) == 10);
	BOOST_CHECK(octet3.Limit(NE) == 14);
	BOOST_CHECK(octet3.Limit(N) == 4);
	BOOST_CHECK(octet3.Limit(NW) == 0);
	BOOST_CHECK(octet3.Limit(W) == -1);
	BOOST_CHECK(octet3.Limit(SW) == -2);
	BOOST_CHECK(octet3.Limit(S) == -1);
	BOOST_CHECK(octet3.Limit(SE) == 6);

	tOctet octet4(octet3);
	BOOST_CHECK(octet4.Limit(E) == 10);
	BOOST_CHECK(octet4.Limit(NE) == 14);
	BOOST_CHECK(octet4.Limit(N) == 4);
	BOOST_CHECK(octet4.Limit(NW) == 0);
	BOOST_CHECK(octet4.Limit(W) == -1);
	BOOST_CHECK(octet4.Limit(SW) == -2);
	BOOST_CHECK(octet4.Limit(S) == -1);
	BOOST_CHECK(octet4.Limit(SE) == 6);
}					 

BOOST_AUTO_TEST_CASE(cover_point)
{
	tPoint point1(10, 4);
	tOctet octet(point1);
	tPoint point2(1, 1); //outer
	octet.CoverPoint(point2);

	BOOST_CHECK(octet.Limit(E) == 10);
	BOOST_CHECK(octet.Limit(NE) == 14);
	BOOST_CHECK(octet.Limit(N) == 4);
	BOOST_CHECK(octet.Limit(NW) == 0);
	BOOST_CHECK(octet.Limit(W) == -1);
	BOOST_CHECK(octet.Limit(SW) == -2);
	BOOST_CHECK(octet.Limit(S) == -1);
	BOOST_CHECK(octet.Limit(SE) == 6);


	tPoint point3(3, 5); //outer
	octet.CoverPoint(point3);

	BOOST_CHECK(octet.Limit(E) == 10);
	BOOST_CHECK(octet.Limit(NE) == 14);
	BOOST_CHECK(octet.Limit(N) == 5);
	BOOST_CHECK(octet.Limit(NW) == 2);
	BOOST_CHECK(octet.Limit(W) == -1);
	BOOST_CHECK(octet.Limit(SW) == -2);
	BOOST_CHECK(octet.Limit(S) == -1);
	BOOST_CHECK(octet.Limit(SE) == 6);

	tPoint point4(3, 4); //inner
	octet.CoverPoint(point4);

	BOOST_CHECK(octet.Limit(E) == 10);
	BOOST_CHECK(octet.Limit(NE) == 14);
	BOOST_CHECK(octet.Limit(N) == 5);
	BOOST_CHECK(octet.Limit(NW) == 2);
	BOOST_CHECK(octet.Limit(W) == -1);
	BOOST_CHECK(octet.Limit(SW) == -2);
	BOOST_CHECK(octet.Limit(S) == -1);
	BOOST_CHECK(octet.Limit(SE) == 6);

	tPoint point5(2, 4); //border
	octet.CoverPoint(point5);

	BOOST_CHECK(octet.Limit(E) == 10);
	BOOST_CHECK(octet.Limit(NE) == 14);
	BOOST_CHECK(octet.Limit(N) == 5);
	BOOST_CHECK(octet.Limit(NW) == 2);
	BOOST_CHECK(octet.Limit(W) == -1);
	BOOST_CHECK(octet.Limit(SW) == -2);
	BOOST_CHECK(octet.Limit(S) == -1);
	BOOST_CHECK(octet.Limit(SE) == 6);
}

BOOST_AUTO_TEST_CASE(isInside_test)
{
	tPoint point1(10, 4);
	tOctet octet(point1);
	tPoint point2(1, 1);
	octet.CoverPoint(point2);
	tPoint point3(3, 5);
	octet.CoverPoint(point3);
	BOOST_CHECK(octet.IsInside(tPoint(4, 3)) == 2);
	BOOST_CHECK(octet.IsInside(tPoint(9, 4)) == 2);
	BOOST_CHECK(octet.IsInside(tPoint(2, 4)) == 1);
	BOOST_CHECK(octet.IsInside(tPoint(9, 5)) == 1);
	BOOST_CHECK(octet.IsInside(tPoint(2, -1)) == 0);
	BOOST_CHECK(octet.IsInside(tPoint(1, 4)) == 0);
}

BOOST_AUTO_TEST_CASE(isFull_test)
{
	tPoint point1(10, 4);
	tOctet octet(point1);
	tPoint point2(1, 1);
	octet.CoverPoint(point2);
	tPoint point3(3, 5);
	octet.CoverPoint(point3);
	BOOST_CHECK(octet.IsFull() == false);
	octet.CoverPoint(tPoint(4, 0));
	BOOST_CHECK(octet.IsFull() == false);
	octet.CoverPoint(tPoint(10, 3));
	BOOST_CHECK(octet.IsFull() == true);
}

BOOST_AUTO_TEST_CASE(hasEdge_test)
{
	tPoint point1(10, 4);
	tOctet octet(point1);
	tPoint point2(1, 1);
	octet.CoverPoint(point2);
	tPoint point3(3, 5);
	octet.CoverPoint(point3);
	BOOST_CHECK(octet.HasEdge(SW) == false);
	BOOST_CHECK(octet.HasEdge(E) == false);

	octet.CoverPoint(tPoint(4, 0));
	BOOST_CHECK(octet.HasEdge(SW) == true);
	BOOST_CHECK(octet.HasEdge(E) == false);
	octet.CoverPoint(tPoint(10, 3));
	BOOST_CHECK(octet.HasEdge(SW) == true);
	BOOST_CHECK(octet.HasEdge(E) == true);
}


BOOST_AUTO_TEST_CASE(isSegment_test)
{
	tPoint point1(10, 4);
	tOctet octet(point1);
	BOOST_CHECK(octet.IsRegularSegment() == false);
	BOOST_CHECK(octet.IsPoint() == true);

	tPoint point2(5, -1);
	octet.CoverPoint(point2);
	BOOST_CHECK(octet.IsRegularSegment() == true);
	BOOST_CHECK(octet.IsPoint() == false);

	tPoint point3(3, 5);
	octet.CoverPoint(point3);
	BOOST_CHECK(octet.IsRegularSegment() == false);
	BOOST_CHECK(octet.IsPoint() == false);
}

BOOST_AUTO_TEST_CASE(neighbour_intersection_test)
{
	tPoint point1(10, 4);
	tPoint point2(1, 1);
	tPoint point3(3, 5);
	tOctet octet(point1);
	octet.CoverPoint(point2);
	octet.CoverPoint(point3);

	tPoint intersection;
	octet.CommonPoint(E, NE, intersection);
	BOOST_CHECK(intersection == tPoint(10, 4));
	octet.CommonPoint(NE, E, intersection);
	BOOST_CHECK(intersection == tPoint(10, 4));

	octet.CommonPoint(N, NE, intersection);
	BOOST_CHECK(intersection == tPoint(9, 5));

	octet.CommonPoint(N, NW, intersection);
	BOOST_CHECK(intersection == tPoint(3, 5));

	octet.CommonPoint(W, NW, intersection);
	BOOST_CHECK(intersection == tPoint(1, 3));
	octet.CommonPoint(W, SW, intersection);
	BOOST_CHECK(intersection == tPoint(1, 1));

	octet.CommonPoint(S, SW, intersection);
	BOOST_CHECK(intersection == tPoint(1, 1));

	octet.CommonPoint(S, SE, intersection);
	BOOST_CHECK(intersection == tPoint(7, 1));

	octet.CommonPoint(E, SE, intersection);
	BOOST_CHECK(intersection == tPoint(10, 4));
}

BOOST_AUTO_TEST_CASE(vertex_test)
{
	tPoint point1(10, 4);
	tPoint point2(1, 1);
	tPoint point3(3, 5);
	tOctet octet(point1);
	octet.CoverPoint(point2);
	octet.CoverPoint(point3);

	BOOST_CHECK(octet.Vertex(eRegDir(E)) == tPoint(10, 4));
	BOOST_CHECK(octet.Vertex(eRegDir(NE)) == tPoint(9, 5));
	BOOST_CHECK(octet.Vertex(eRegDir(N)) == tPoint(3, 5));
	BOOST_CHECK(octet.Vertex(eRegDir(NW)) == tPoint(1, 3));
	BOOST_CHECK(octet.Vertex(eRegDir(W)) == tPoint(1, 1));
	BOOST_CHECK(octet.Vertex(eRegDir(SW)) == tPoint(1, 1));
	BOOST_CHECK(octet.Vertex(eRegDir(S)) == tPoint(7, 1));
	BOOST_CHECK(octet.Vertex(eRegDir(SE)) == tPoint(10, 4));
}

BOOST_AUTO_TEST_CASE(inflate_point_test)
{
	tPoint point1(10, 4);
	tOctet octet(point1);

	octet.Inflate(4);
	
	BOOST_CHECK(octet.Limit(eRegDir(E)) == 14);
	BOOST_CHECK(octet.Limit(eRegDir(NE)) == 20);
	BOOST_CHECK(octet.Limit(eRegDir(N)) == 8);
	BOOST_CHECK(octet.Limit(eRegDir(NW)) == 0);
	BOOST_CHECK(octet.Limit(eRegDir(W)) == -6);
	BOOST_CHECK(octet.Limit(eRegDir(SW)) == -8);
	BOOST_CHECK(octet.Limit(eRegDir(S)) == 0);
	BOOST_CHECK(octet.Limit(eRegDir(SE)) == 12);
	octet.Inflate(-4);
	for (auto i = 0; i < 8; ++i) {
		BOOST_CHECK(octet.Limit(eRegDir(i)) == point1.Limit(eRegDir(i)));
	}
}


BOOST_AUTO_TEST_SUITE_END()