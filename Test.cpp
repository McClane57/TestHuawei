#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>

#include "tPoint.h"
#include "tOctet.h"
#include "limits.h"
#include "tSegment.h"
#include "tTrajectory.h"
#include "tShadow.h"


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

BOOST_AUTO_TEST_CASE(directional_segment_test)
{
	tPoint start(10, 10);
	BOOST_CHECK(tSegment(start, E, 10).End(1) == tPoint(20, 10));
	BOOST_CHECK(tSegment(start, NE, 10).End(1) == tPoint(15, 15));
	BOOST_CHECK(tSegment(start, N, 10).End(1) == tPoint(10, 20));
	BOOST_CHECK(tSegment(start, NW, 10).End(1) == tPoint(5, 15));
	BOOST_CHECK(tSegment(start, W, 10).End(1) == tPoint(0, 10));
	BOOST_CHECK(tSegment(start, SW, 10).End(1) == tPoint(5, 5));
	BOOST_CHECK(tSegment(start, S, 10).End(1) == tPoint(10, 0));
	BOOST_CHECK(tSegment(start, SE, 10).End(1) == tPoint(15, 5));
}

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

BOOST_AUTO_TEST_CASE(swap_test)
{
	tPoint start1(20, -30);
	tPoint finish1(50, -60);

	tSegment segment1(start1, finish1);
	segment1.SwapEnds();
	BOOST_CHECK(segment1.End(0) == finish1);
	BOOST_CHECK(segment1.End(1) == start1);
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
	BOOST_CHECK(segment6.Directions().first == N);
	BOOST_CHECK(segment6.Directions().second == NW);
}

BOOST_AUTO_TEST_CASE(align_critical_test)
{
	tSegment segment1(tPoint(0, 0), tPoint(INT32_MAX, 1));
	BOOST_CHECK(segment1.IsPointOnLine(tPoint(INT32_MIN, -1)) == 1);
	BOOST_CHECK(segment1.IsPointOnLine(tPoint(INT32_MIN + 1, -1)) == 0);
	BOOST_CHECK(segment1.IsPointOnLine(tPoint(INT32_MIN + 1000, -1)) == -1);
}

BOOST_AUTO_TEST_CASE(segment_align_test)
{
	tSegment segment1(tPoint(0, 0), tPoint(10, 1));
	tSegment segment2(tPoint(-21, -2), tPoint(-11, -1));
	tSegment segment3(tPoint(-22, -2), tPoint(-11, -1));
	tSegment segment4(tPoint(-20, -2), tPoint(-10, -1));
	tSegment segment5(tPoint(-18, -2), tPoint(-9, -1));



	BOOST_CHECK(segment1.IsAlign(segment2) == 1);
	BOOST_CHECK(segment1.IsAlign(segment3) == 1);
	BOOST_CHECK(segment1.IsAlign(segment4) == 0);
	BOOST_CHECK(segment1.IsAlign(segment5) == -1);
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

	tPoint point1(10, 4);
	tPoint point2(1, 1);
	tSegment segment2(point1, point2, 4);
	tOctet octet5(segment2);
	BOOST_CHECK(octet5.Limit(eRegDir(E)) == 14);
	BOOST_CHECK(octet5.Limit(eRegDir(NE)) == 20);
	BOOST_CHECK(octet5.Limit(eRegDir(N)) == 8);
	BOOST_CHECK(octet5.Limit(eRegDir(NW)) == 6);
	BOOST_CHECK(octet5.Limit(eRegDir(W)) == 3);
	BOOST_CHECK(octet5.Limit(eRegDir(SW)) == 4);
	BOOST_CHECK(octet5.Limit(eRegDir(S)) == 3);
	BOOST_CHECK(octet5.Limit(eRegDir(SE)) == 12);
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

BOOST_AUTO_TEST_CASE(inflate_segment_test)
{
	tPoint point1(10, 4);
	tPoint point2(1, 1);
	tSegment segment(point1, point2);
	tOctet octet(segment);
	octet.Inflate(4);

	BOOST_CHECK(octet.Limit(eRegDir(E)) == 14);
	BOOST_CHECK(octet.Limit(eRegDir(NE)) == 20);
	BOOST_CHECK(octet.Limit(eRegDir(N)) == 8);
	BOOST_CHECK(octet.Limit(eRegDir(NW)) == 6);
	BOOST_CHECK(octet.Limit(eRegDir(W)) == 3);
	BOOST_CHECK(octet.Limit(eRegDir(SW)) == 4);
	BOOST_CHECK(octet.Limit(eRegDir(S)) == 3);
	BOOST_CHECK(octet.Limit(eRegDir(SE)) == 12);
	octet.Inflate(-4);
	for (auto i = 0; i < 8; ++i) {
		BOOST_CHECK(octet.Limit(eRegDir(i)) == segment.Limit(eRegDir(i)));
	}
	octet.Inflate(-1);
	BOOST_CHECK(octet.Limit(eRegDir(E)) == 9);
	BOOST_CHECK(octet.Limit(eRegDir(NE)) == 12);
	BOOST_CHECK(octet.Limit(eRegDir(N)) == 3);
	BOOST_CHECK(octet.Limit(eRegDir(NW)) == -2);
	BOOST_CHECK(octet.Limit(eRegDir(W)) == -2);
	BOOST_CHECK(octet.Limit(eRegDir(SW)) == -4);
	BOOST_CHECK(octet.Limit(eRegDir(S)) == -2);
	BOOST_CHECK(octet.Limit(eRegDir(SE)) == 4);
}

BOOST_AUTO_TEST_CASE(octet_intersect_test)
{
	tPoint point1(10, 4);
	tPoint point2(1, 1);
	tSegment segment(point1, point2);
	tOctet octet1(segment);

	tPoint point3(4, 10);
	tOctet octet2(point3);

	BOOST_CHECK(octet1.Intersect(&octet2) == nullptr);
	BOOST_CHECK(octet2.Intersect(&octet1) == nullptr);
	BOOST_CHECK(*octet1.Intersect(&octet1) == octet1);
	BOOST_CHECK(*octet2.Intersect(&octet2) == octet2);

	octet2.Inflate(8);
	BOOST_CHECK(*octet1.Intersect(&octet2) == tOctet({ 10,14,4,0,-2,-4,-2,6 }));

	octet2.Inflate(-2);
	BOOST_CHECK(octet1.Intersect(&octet2)->IsRegularSegment() == true);
}


BOOST_AUTO_TEST_CASE(cure_test)
{
	tOctet octet1({ 10,14,4,0,-2,-4,-2,6 });
	octet1.Cure();
	BOOST_CHECK(octet1 == tOctet({ 10,14,4,0,-2,-4,-2,6 }));

	tOctet octet2({ 10,14,4,0,-1,-4,-2,6 });
	octet2.Cure();
	BOOST_CHECK(octet2 == tOctet({ 10,14,4,0,-2,-4,-2,6 }));

	tOctet octet3({ 1,10,1,10,1,10,1,10 });
	octet3.Cure();
	BOOST_CHECK(octet3 == tOctet({ 1,2,1,2,1,2,1,2 }));

	tOctet octet4({ 1,10,20,10,1,10,1,10 });
	octet4.Cure();
	BOOST_CHECK(octet4 == tOctet({ 1,10,10,10,1,2,1,2 }));

	tOctet octet5({ 2,2,4,2,0,6,4,2 });
	octet5.Cure();
	BOOST_CHECK(octet5 == tOctet({ 2,2,2,2,0,2,2,2 }));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(trajectory_tests)


BOOST_AUTO_TEST_CASE(regular_trajectory_test)
{
	tTrajectory trajectory1;
	trajectory1.AddPoint(tPoint(1, 1));
	trajectory1.AddPoint(tPoint(10, 10));
	trajectory1.AddPoint(tPoint(10, 3));
	BOOST_CHECK(trajectory1.IsRegular() == true);

	tTrajectory trajectory2;
	trajectory2.AddPoint(tPoint(1, 1));
	trajectory2.AddPoint(tPoint(10, 3));
	trajectory2.AddPoint(tPoint(10, 10));

	BOOST_CHECK(trajectory2.IsRegular() == false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(shadow_tests)

BOOST_AUTO_TEST_CASE(shadow_fill_test)
{
	tShadow shadow0;
	tShadow shadow1(NE, S, tPoint(10, 0));
	shadow0 = shadow1;
	bool added = shadow0.AddObstacle(tOctet(tPoint(4, 2)));
	BOOST_CHECK(added);
	BOOST_CHECK(shadow0.Map().size() == 2);
	added = shadow0.AddObstacle(tOctet(tPoint(4, 2)));
	BOOST_CHECK(!added);
	added = shadow0.AddObstacle(tOctet(tPoint(3, 2)));
	BOOST_CHECK(!added);
	added = shadow0.AddObstacle(tOctet(tPoint(3, 3)));
	BOOST_CHECK(added);
	BOOST_CHECK(shadow0.Map().size() == 2);


	added = shadow0.AddObstacle(tOctet(tPoint(1, 4)));
	BOOST_CHECK(added);
	BOOST_CHECK(shadow0.Map().size() == 3);

	added = shadow0.AddObstacle(tOctet(tPoint(0, 4)));
	BOOST_CHECK(!added);
	BOOST_CHECK(shadow0.Map().size() == 3);

	added = shadow0.AddObstacle(tOctet(tPoint(9, 4)));
	BOOST_CHECK(added);
	BOOST_CHECK(shadow0.Map().size() == 2);
}

BOOST_AUTO_TEST_CASE(shadow_point_diagonal_test)
{
	tShadow shadow(NE, S, tPoint(20, 0));
	shadow.AddObstacle(tOctet(tPoint(3, 8)));
	shadow.AddObstacle(tOctet(tPoint(15, 2)));
	shadow.AddObstacle(tOctet(tPoint(10, 5)));
	BOOST_CHECK(shadow.Map().size() == 4);
	BOOST_CHECK(shadow.NextPoint(tPoint(0, 10)) == tPoint(2, 8));
	BOOST_CHECK(shadow.NextPoint(tPoint(1, 10)) == tPoint(6, 5));
	BOOST_CHECK(shadow.NextPoint(tPoint(0, 18)) == tPoint(18, 0));
}

BOOST_AUTO_TEST_CASE(shadow_point_orthogonal_test)
{
	tShadow shadow(E, SW, tPoint(20, 0));
	shadow.AddObstacle(tOctet(tPoint(17, 4)));
	shadow.AddObstacle(tOctet(tPoint(16, 6)));
	shadow.AddObstacle(tOctet(tPoint(15, 8)));
	BOOST_CHECK(shadow.Map().size() == 4);
	BOOST_CHECK(shadow.NextPoint(tPoint(13,13)) == tPoint(13,10));
	BOOST_CHECK(shadow.NextPoint(tPoint(15, 10)) == tPoint(15, 7));
	BOOST_CHECK(shadow.NextPoint(tPoint(18, 5)) == tPoint(18, 2));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ladder_tests)

BOOST_AUTO_TEST_CASE(under_segment_test)
{
	tSegment diagonal1(tPoint(10, 0), tPoint(0, 5));
	tLadder ladder1(diagonal1);
	BOOST_CHECK(ladder1.LastPointUnderDiagonal(tPoint(3, 2)) == tPoint(6, 2));
	BOOST_CHECK(ladder1.LastPointUnderDiagonal(tPoint(2, 2)) == tPoint(6, 2));
	BOOST_CHECK(ladder1.LastPointUnderDiagonal(tPoint(0, 0)) == tPoint(10, 0));



	tSegment diagonal2(tPoint(0, 0), tPoint(10, 6));
	tLadder ladder2(diagonal2);
	BOOST_CHECK(ladder2.LastPointUnderDiagonal(tPoint(3, 1)) == tPoint(5, 3));
	BOOST_CHECK(ladder2.LastPointUnderDiagonal(tPoint(5, 1)) == tPoint(10, 6));
	BOOST_CHECK(ladder2.LastPointUnderDiagonal(tPoint(3, 0)) == tPoint(7, 4));
}

BOOST_AUTO_TEST_CASE(trajectory_builder_no_obstacles_test)
{
	tSegment diagonal1(tPoint(10, 0), tPoint(0, 5));
	tLadder ladder1(diagonal1);
	BOOST_CHECK(ladder1.MakeLadder() == tTrajectory({tPoint(0,5), tPoint(5, 0), tPoint(10, 0)}));

	tSegment diagonal2(tPoint(0, 0), tPoint(10, 6));
	tLadder ladder2(diagonal2);
	BOOST_CHECK(ladder2.MakeLadder() == tTrajectory({ tPoint(0,0), tPoint(4, 0), tPoint(10, 6) }));
}

BOOST_AUTO_TEST_CASE(trajectory_builder_one_obstacle_test)
{
	tSegment diagonal1(tPoint(10, 0), tPoint(0, 5));
	tLadder ladder1(diagonal1);
	ladder1.AddObstacle(tOctet(tPoint(4, 2)));
	BOOST_CHECK(ladder1.MakeLadder() == tTrajectory({ tPoint(0,5), tPoint(3,2), tPoint(6,2), tPoint(8, 0), tPoint(10, 0) }));

	tSegment diagonal2(tPoint(0, 0), tPoint(10, 6));
	tLadder ladder2(diagonal2);
	ladder2.AddObstacle(tOctet(tPoint(5,3)));
	BOOST_CHECK(ladder2.MakeLadder() == tTrajectory({ tPoint(0,0), tPoint(2,0), tPoint(5, 3), tPoint(7,3),  tPoint(10, 6) }));
}

BOOST_AUTO_TEST_CASE(trajectory_builder_few_obstacles_test)
{
	tSegment diagonal1(tPoint(10, 0), tPoint(0, 5));
	tLadder ladder1(diagonal1);
	ladder1.AddObstacle(tOctet(tPoint(4, 2)));
	ladder1.AddObstacle(tOctet(tPoint(7, 1)));
	BOOST_CHECK(ladder1.MakeLadder() == tTrajectory({ tPoint(0,5), tPoint(3,2), tPoint(6,2), tPoint(8, 0), tPoint(10, 0) }));
	ladder1.AddObstacle(tOctet(tPoint(8, 1)));
	BOOST_CHECK(ladder1.MakeLadder() == tTrajectory({ tPoint(0,5), tPoint(3,2), tPoint(6,2), tPoint(7, 1), tPoint(8,1), tPoint(9,0), tPoint(10, 0) }));
}

BOOST_AUTO_TEST_CASE(trajectory_builder_bad_obstacle_test)
{
	tSegment diagonal1(tPoint(10, 0), tPoint(0, 5));
	tLadder ladder1(diagonal1);
	ladder1.AddObstacle(tOctet(tPoint(5, 3)));
	BOOST_CHECK(ladder1.MakeLadder() == tTrajectory({ tPoint(0,5), tPoint(5, 0), tPoint(10, 0) }));
	ladder1.AddObstacle(tOctet(tPoint(11, -1)));
	BOOST_CHECK(ladder1.MakeLadder() == tTrajectory({ tPoint(0,5), tPoint(5, 0), tPoint(10, 0) }));
}

BOOST_AUTO_TEST_CASE(segments_sort_test)
{
	tSegment segment1(tPoint(0, 0), tPoint(10, 10));
	tSegment segment2(tPoint(20, 20), tPoint(5, -10));
	tSegment segment3(tPoint(14, -20), tPoint(-8, 1));
	tSegment segment4(tPoint(8, 10), tPoint(1, 10));
	std::list<tSegment> segments{ segment1,segment2,segment3, segment4 };
	SortSegments(segments);
	BOOST_CHECK(segments.front() == segment3);
	BOOST_CHECK(segments.back() == segment4);
}

BOOST_AUTO_TEST_SUITE_END()