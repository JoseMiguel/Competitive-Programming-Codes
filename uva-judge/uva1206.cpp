#include<iostream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<cstdio>
#include<cmath>

using namespace std ;

const double pi = acos(-1.0);
const double EPS = 1e-9;
struct Point{
	double x, y;
	double ang;
	Point(){}
	Point( double x, double y ):x(x),y(y){}
	bool operator < ( const Point &p ) const {
		if ( ang == p.ang ){
			if ( x == p.x ) return y < p.y;
			return x < p.x;
		}
		return ang > p.ang;
	}
	double operator %( Point p ) {
		return x * p.y - y * p.x;
	}
	double operator *( Point p ){
		return x * p.x + y * p.y;
	}

	Point operator *( double k ){
		return Point( x * k , y * k );
	}

	Point operator + ( Point p ){
		return Point( p.x + x , p.y + y );
	}
	Point operator - ( Point p ){
		return Point( x - p.x, y - p.y);
	}
	Point operator / ( double d ){
		return Point( x / d , y / d );
	}
	
};
typedef vector<Point> Poly;

double ccw( Point a, Point b, Point c ){
	return ( b - a ) % ( c - a );
}

double norma( Point a ) {
	return hypot( a.x, a.y );
}

double dist( Point a, Point b ){
	return norma( b - a );
}

//distancia perpendicular del punto p al segmento a,b 
double dist( Point p, Point a, Point b ){
	return ccw( p , a , b ) / norma( b - a );
}
// Ax + By = C
struct Recta{
	double A, B, C;
	Recta(){}
	Recta( Point a, Point b ){
		A = b.y - a.y;
		B = a.x - b.x;
		C = A*a.x + B*a.y;
	}
	Point intersect( Recta r, bool &paralelas ) {
		double det = A*r.B - r.A*B;
		if ( det == 0 ) {
			paralelas = true;
			return Point(1<<20,1<<20);
		}else{
			double x = (r.B*C - B*r.C) / det;
			double y = (A*r.C - r.A*C) / det;
			return Point( x, y );
		}
	}
};


//Poligonos

double area( Poly p ){
	double res = 0.0;
	int n = p.size();
	for( int i = 0; i < n; i++){
		res += p[ i ] % p[ ( i + 1 ) % n ];
	}
	return fabs(res / 2.0 );
}

Point centerMass( Poly p ){
	Point c( 0 , 0 ) ;
	for( int i = 0; i < p.size(); i++ ) {
		c.x += p[i].x / (double)p.size();
		c.y += p[i].y / (double)p.size();
	}
	return c;
}

// convex hull 

Poly convexHull( Poly p ){
	sort( p.begin(), p.end() );
	int n = p.size(), k = 0;
	Poly h ( 2 * n );
	for( int i = 0; i < n; i++ ){
		while( k >= 2 && ccw( h[k-2], h[k-1], p[i] ) <= 0 ) k--;
		h[k++] = p[i];
	}
	int t = k + 1;
	for( int i = n - 2; i >= 0; i-- ){
		while( k >= t && ccw( h[k-2], h[k-1], p[i] ) <= 0 ) k--;
		h[k++] = p[i];
	}
	// devuelve el primero y el ultimo punto iguales, por eso se le resta 1
	h.resize( k - 1 );
	return h;
	
}

// si un punto esta dentro de un poligono

bool inPoly( Poly &P, Point p ){
	int cross = 0, n = P.size();
	for( int i = 1; i <= n; i++){
		Point q = P[i-1]; 
		Point r = P[i%n]; 

		// qr es un lado del poligono
		if ( q.y > r.y ) swap( q, r ); // por comodidad hacer que r siempre este arriba de q
		double ori = ccw( p ,q , r ); // la orientacion 
		if ( p.y > q.y && p.y <= r.y ){ // esta tipo al medio de un segmento... el intervalo debe ser asi... (q, r] 
		// de tal forma que en la siguiente iteracion no se repita el r
			if ( ori > 0 ) { // si el segmento esta tipo a la derecha 
				cross++;
			}
			else if ( ori == 0 ) return true;
		}
	}
	return cross % 2;

}

void simplePolygon( Poly &p ){
	Point c = centerMass( p ) ;
	for( int i = 0; i < p.size(); i++){
		p[i].ang = atan2( c.x - p[i].x , c.y - p[i].y );
	}
	sort( p.begin(), p.end() );
}

int main(){
	double x , y;
	string s; char c;
	while( getline( cin , s ) ){
		Poly P;
		istringstream is ( s );
		while( is >> c >> x >> c >> y >> c ){
			P.push_back( Point( x, y ) );
		}
		P = convexHull( P );
		for( int i = 0; i < P.size(); i++){
			if ( i ) cout<< " ";
			cout << "("<<P[i].x << "," << P[i].y<<")";
		}
		cout << endl;
	}
}


