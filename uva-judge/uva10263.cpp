#include<iostream>
#include<cstring>
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
	return sqrt( a.x*a.x + a.y*a.y );
}

double dist( Point a, Point b ){
	return norma( b - a );
}

//distancia minima del punto p al segmento a,b 
double dist( Point p, Point a, Point b, bool &mod, Point e ){
	
	Point ba = a - b;
	Point ab = b - a;
	Point bp = p - b;
	Point ap = p - a;
	mod = false;	
	if ( ba * bp < 0 ) {
		mod = true;
		e = b;
		return dist( b, p );
	}
	if ( ab * ap < 0 ){
		mod = true;
		e = a;
		return dist( a, p );
	}
	return fabs(ccw( p , a , b )) / norma( b - a );
}

int ccw2( Point a, Point b, Point c ){
	Point ba = b - a;
	Point ca = c - a;
	if ( (ba % ca) > 0 ) return 1;
	if ( (ba % ca) < 0 ) return -1;
	if ( (ba * ca) < 0 ) return 2;
	if ( norma(ba) < norma(ca) ) return -2;
	return 0;
}

bool intersect( Point a1, Point a2, Point b1, Point b2 ){
	return ccw2( a1, a2, b1 ) * ccw2( a1, a2,b2) <= 0 &&
	ccw2(b1,b2,a1) * ccw2(b1,b2,a2) <= 0 ;
}

Point crossPoint( Point a1, Point a2, Point b1, Point b2 ) {
	// chekar si se intersecan antes
	double A = (a2-a1) % (b2-b1) ;
	double B = (a2-a1) % (a2-a1) ;

	double t = B / A;
	if ( fabs(A) < EPS && fabs(B) < EPS ) return b1;
	return b1 + (b2 - b1) * t; // P(t) = a + ( b-a ) * t
}


Point rotate( Point p , double ang ){
	return Point( cos( ang ) * p.x - sin( ang ) * p.x , sin( ang ) * p.x + cos( ang ) * p.y );
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

double area( Poly &p ){
	double res = 0.0;
	int n = p.size();
	for( int i = 0; i < n; i++){
		res += p[ i ] % p[ ( i + 1 ) % n ];
	}
	return fabs(res / 2.0 );
}

Point centerMass( Poly &p ){
	Point c( 0 , 0 ) ;
	for( int i = 0; i < p.size(); i++ ) {
		c.x += p[i].x ;/// (double)p.size();
		c.y += p[i].y ;/// (double)p.size();
	}
	return c / (double) p.size();
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

// punto dentro de un triangualo

bool inTriang( const Poly &T , const Point &p){
	return ccw2( T[0], T[1], p ) >= 0 && 
		ccw2( T[1],T[2],p ) >= 0 && 
		ccw2( T[2],T[0],p) >= 0; 
}
// triangular un poligono en O(2*n^2) devuelve un vector de triangulos
bool can( const Poly &t, const  Poly &p, int i , int j , int k ){
	if(ccw(t[0], t[1], t[2]) <= 0) return false;
	for(int l = 0; l < p.size(); l++){
		if(l != i && l != j && l != k)
			if( inTriang(t, p[l]))	return false;
	}
	return true;
}
vector<Poly> triangulate( const Poly &p ){
	vector<Poly> res;
	int n = p.size();
	vector<int> l, r;
	for( int i = 0; i < n; i++){
		l.push_back( ( i - 1 + n) % n );
		r.push_back( ( i + 1) % n );
	}
	int i = n - 1, cagao = 0;
	while( res.size() < n - 2 ){
		if ( cagao >= n ) return vector<Poly>();
		i = r[i];
		Poly tmp;
		tmp.push_back( p[l[i]] );
		tmp.push_back( p[i] );
		tmp.push_back( p[r[i]] );

		if ( can( tmp, p , l[i], i , r[i] ) ){
			res.push_back( tmp );
			l[ r[i] ] = l[i];
			r[ l[i] ] = r[i];
			cagao = 0;
		}else cagao++;
	}
	return res;
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

	int x, y;
	while( scanf("%d %d",&x, &y ) != -1 ){
		Point m ( x, y );
		int n;
		scanf("%d",&n);
		double res = 0.0;
		Poly p( n + 1 );
		for( int i = 0; i <= n; i++){
			scanf("%lf %lf",&p[i].x,&p[i].y);
		//	cout << p[i].x <<" " << p[i].y << endl;
		}
		int id;
		double d = 1e12;
		bool ext = false;
		Point e,E;
		double tmp;
		bool mod = false;

		for( int i = 1; i <= n; i++){
			tmp = dist( p[i-1], p[i%n], m, mod, E );
			if( tmp < d ){
				d = tmp;
				id = i;
				ext = mod;
				e = E;
			printf("%.4lf ", e.x);
			printf("%.4lf\n", e.y);
			Point a = p[id-1];
			Point b = p[id];

			Point aM = ( m - a );
			Point ab = ( b - a );
			double h = dist( a , m );

			double t = sqrt( h*h - d*d + 2*d*h*( (aM*ab)/(norma(aM)*norma(ab)) ));
			Point res = a + (( b - a )* t);

			printf("%.4lf ", res.x);
			printf("%.4lf\n", res.y);
			}
		}
	
		

		

	}

	return 0;
}

