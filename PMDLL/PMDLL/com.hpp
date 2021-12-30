#pragma once
#include <stdio.h>  
#include <stdlib.h> 
#include <stdarg.h>
#include <math.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <windows.h>

// **********************************************************************
// macro
// **********************************************************************
#define PRINT(msg)			MYTRACE(msg)//{std::stringstream ss; ss << "\n> " << msg; printf(ss.str().c_str());}
#define PRINTV(v)			PRINT(#v << " = " << v)
#define PRINTVEC(vm, v)		PRINT(vm << " = " << v.x << "," << v.y << "," << v.z)
#define PRINTVEC2(v)		PRINT(#v << " = " << v.x << "," << v.y)
#define PRINTVEC3(v)		PRINT(#v << " = " << (v).x << "," << (v).y << "," << (v).z)
#define PRINTVEC4(v)		PRINT(#v << " = " << v.x << "," << v.y << "," << v.z << "," << v.w)
#define PRINTTRI(p1,p2,p3)	PRINT("tri" << "(" << p1.x << "," << p1.y << "," << p1.z << "," << p2.x << "," << p2.y << "," << p2.z << "," << p3.x << "," << p3.y << "," << p3.z << ")")
#define PRINTCVP(p)			PRINT("cvp(" << 50 * p.x + 500 << "," << 50 * p.y + 500 << "," << 50 * p.z + 500 << ")")

#define MSGBOX(msg)		{std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0,ss.str().c_str(),"MSG", 0);}
#define MYTRACE(msg)	{std::stringstream ss; ss << "\n" << msg; printf(ss.str().c_str()); ::OutputDebugStringA(ss.str().c_str());}
#define LOGMSG(msg)		{if(gblog) {logmessage << msg; }}
#define event(obj, msg)	{if(gblog) logmessage << "\n" << obj << " : " << msg;}
#define ASSERT(x)		{if(!(x)){std::stringstream ss; ss << "ASSERT FAILED! " << __FILE__ << "(" << __LINE__ << ")"; ::MessageBoxA(0, ss.str().c_str(), "ASSERT", 0); throw;} }
#define ERROR(x)		{if(!(x)){std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0, ss.str().c_str(), "ERROR", 0);} }
#define ERRORMSG(msg)	{std::stringstream ss; ss << "\n" << msg; ::MessageBoxA(0, ss.str().c_str(), "ERROR", 0);}

#define real			float
#define crstr			const std::string&
#define	EPSINON			0.0005
#define PI				3.1415926
#define	IMAGESCALE		2048
#define	FIMAGESCALE		((real)IMAGESCALE)
#define	UNIT			(1.0f / FIMAGESCALE)
//#define MAXZ			0.0

#define RGB(r, g, b)	((int)(((unsigned char)(r)) | (((short)(unsigned char)(g)) << 8) | (((int)(unsigned char)(b)) << 16)))
#define GetRValue(rgb)	((unsigned char)(0x000000ff & (rgb)))
#define GetGValue(rgb)	((unsigned char)((0x0000ff00 & (rgb))>>8))
#define GetBValue(rgb)	((unsigned char)((0x00ff0000 & (rgb))>>16))
#define MIN(a, b)		((a) < (b) ? (a) : (b))
#define MAX(a, b)		((a) > (b) ? (a) : (b))
#define vec				vector3
#define crvec			const vector3&
#define rvec			vector3&
#define vec2			vector2
#define vec3			vector3

#define LOOP(i, szvar, szval) const int szvar = szval; for(int i = 0; i <= szvar; i ++){real a##i = i / real(szvar);	
#define FOR(i, szvar, szval) const int szvar = szval; for(int i = 0; i < szvar; i ++){real a##i = i / real(szvar);	
#define END	}
#define RNDBW	blendcor(1, 0xFF808080, rrnd(0.25, 1))
#define RNDCOR	RGB(rrnd(0, 255), rrnd(0, 255), rrnd(0, 255))
#define VECLIST	std::vector<vertex>

#define E	0.61828183
#define E1	(E)
#define E2	(E1 * E)
#define E3	(E2 * E)
#define E4	(E3 * E)
#define E5	(E4 * E)
#define E6	(E5 * E)
#define E7	(E6 * E)
#define E8	(E7 * E)
#define E9	(E8 * E)

#define PUSH	push_back
#define	CIRCLE(d_i, r, n)	LOOP(d_i, len##d_i, n)real ang##d_i = a##d_i * PI * 2 + PI / n;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));
#define	CIRCLE2(d_i, r, n, ang0)	LOOP(d_i, len##d_i, n)real ang##d_i = a##d_i * PI * 2 + ang0;	vec2 dp##d_i((r) * cos(ang##d_i), (r) * sin(ang##d_i));
#define BEGINDRAW renderstate = 1;
#define ENDDRAW renderstate = 2;
#define DEF_SCALES(s) 	real s1 = s  * E * E;real s2 = s1 * E * E;real s3 = s2 * E * E;real s4 = s3 * E * E;real s5 = s4 * E * E;real s6 = s5 * E * E;real s7 = s6 * E * E;real s8 = s7 * E * E;
#define DEF_SCALES2(s) 	real s1 = s  * E;real s2 = s1 * E;real s3 = s2 * E;real s4 = s3 * E;real s5 = s4 * E;real s6 = s5 * E;real s7 = s6 * E;real s8 = s7 * E;
#define START_MESH	resetsm();color = 0xFFFFFFFF;
#define END_MESH exportsm();
#define	VEC2STR(v)	(v).x << ", " << (v).y << ", " << (v).z

// ----------------------------------------------------------------------
inline real rrnd(real min = 0.0, real max = 1.0) { return (max) == (min) ? (min) : (min)+((real)(rand()) / (RAND_MAX + 1.0)) * ((max)-(min)); }
// ----------------------------------------------------------------------
real MAXZ = 0.0;
real EXPORT_SCALE = 10.0;
real USIZE0 = 0.04f;
real USIZE = USIZE0 / EXPORT_SCALE;
#define MINSTEP  USIZE / rrnd(20.0f, 50.0f)
bool gblog = true;
std::stringstream logmessage;

// **********************************************************************
// 2D
// **********************************************************************
struct vector2{
	union{
		real val[2];
		struct{
			real x;
			real y;
		};
	};
	
	static const vector2 ZERO;
	static const vector2 UX;
	static const vector2 UY;
	static const vector2 CENTER;
	
	real& operator [](int ind){
		return val[ind];
	}

	vector2(){
		x = 0;
		y = 0;
	}
	vector2(real _x, real _y){
		x = _x;
		y = _y;
	}
	vector2 operator + (const vector2& _p) const
	{
		vector2 fp;
		fp.x = x + _p.x;
		fp.y = y + _p.y;

		return fp;
	}
	vector2 operator - (const vector2& _p) const
	{
		vector2 fp;
		fp.x = x - _p.x;
		fp.y = y - _p.y;
		return fp;
	}
	vector2 operator - () const
	{
		vector2 fp;
		fp.x = - x;
		fp.y = - y;
		return fp;
	}

	vector2 operator * (real s) const
	{
		vector2 fp;
		fp.x = s * x;
		fp.y = s * y;
		return fp;		
	}
	vector2 operator * (const vector2& b) const
	{
		return vector2(x * b.x - y * b.y, x * b.y + y * b.x);	
	}
	vector2 operator / (real s) const
	{
		vector2 fp;
		fp.x = x / s;
		fp.y = y / s;
		return fp;		
	}
	real len() const
	{
		return sqrt(x * x + y * y);
	}
	real sqrlen() const
	{
		return (x * x + y * y);
	}
	real angle() const
	{
		return atan2(y, x);
	}
	void norm()
	{
		real r = len();
		if(r > 0)
		{
			x /= r;
			y /= r;
		}	
	}
	vector2 normcopy() const
	{
		real r = len();
		if(r > 0)
		{
			return vector2( x / r, y / r);
		}	
		return vector2::ZERO;
	}
	void rot(real angle)
	{
		(*this) = (*this) * vector2::fromanglelength(angle, 1);		
	}
	vector2 rotcopy(real angle) const
	{
		return (*this) * vector2::fromanglelength(angle, 1);		
	}
	void rot(real angle, const vector2& o)
	{
		vector2 v = (*this) - o;
		v = v * vector2::fromanglelength(angle, 1);
		(*this) = v + o;
	}
	vector2 rotcopy(real angle, const vector2& o) const
	{
		vector2 v = (*this) - o;
		v = v * vector2::fromanglelength(angle, 1);
		return v + o;	
	}	
	real dot(const vector2& v) const
	{
		return x * v.x + y * v.y;
	}
	real cross(const vector2& v) const
	{
		return x * v.y - y * v.x;
	}
	static vector2 fromanglelength(real _angle, real _r);
};

const vector2 vector2::ZERO = vector2(0, 0);
const vector2 vector2::UX = vector2(1, 0);
const vector2 vector2::UY = vector2(0, 1);
const vector2 vector2::CENTER = vector2(0.5, 0.5);

vector2 vector2::fromanglelength(real _angle, real _r)
{
	return vector2(_r * cos(_angle), _r * sin(_angle));
}
// **********************************************************************
// 3D
// **********************************************************************
struct vector3
{
	static const vector3 ZERO;
	static const vector3 UX;
	static const vector3 UY;
	static const vector3 UZ;
	static const vector3 CENTER;
	static real sEPSINON;
	union{
		real val[3];
		struct
		{
			float x;
			float y;
			float z;
		};
	};
	vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	real& operator [](int ind){
		return val[ind];
	}
	vector3 operator + (const vector3& _p) const
	{
		vector3 fp;
		fp.x = x + _p.x;
		fp.y = y + _p.y;
		fp.z = z + _p.z;
		return fp;
	}
	void operator += (const vector3& _p)
	{	
		x += _p.x;
		y += _p.y;
		z += _p.z;	
	}
	vector3 operator - (const vector3& _p) const
	{
		vector3 fp;
		fp.x = x - _p.x;
		fp.y = y - _p.y;
		fp.z = z - _p.z;
		return fp;
	}
	void operator -= (const vector3& _p)
	{	
		x -= _p.x;
		y -= _p.y;
		z -= _p.z;	
	}
	vector3 operator - () const
	{
		vector3 fp;
		fp.x = - x;
		fp.y = - y;
		fp.z = - z;
		return fp;
	}
	vector3 operator * (float s) const
	{
		vector3 fp;
		fp.x = s * x;
		fp.y = s * y;
		fp.z = s * z;
		return fp;
	}
	void operator *= (float s)
	{		
		x = s * x;
		y = s * y;
		z = s * z;		
	}
	vector3 operator / (float s) const
	{
		vector3 fp;
		fp.x = x / s;
		fp.y = y / s;
		fp.z = z / s;
		return fp;
	}
	void operator /= (float s)
	{	
		x = x / s;
		y = y / s;
		z = z / s;	
	}	
	bool operator == (const vector3& rv) const
	{
		return (fabs(x-rv.x) <= sEPSINON && fabs(y-rv.y) <= sEPSINON && fabs(z-rv.z) <= sEPSINON);
	}
	bool operator != (const vector3& rv) const
	{
		return (fabs(x - rv.x) > sEPSINON || fabs(y - rv.y) > sEPSINON || fabs(z - rv.z) > sEPSINON);
	}
	bool operator < ( const vector3& rv ) const
	{
		if( x < rv.x && y < rv.y && z < rv.z )
			return true;
		return false;
	}
	bool operator > ( const vector3& rv ) const
	{
		if( x > rv.x && y > rv.y && z > rv.z )
			return true;
		return false;
	}
	float len() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	float sqrlen() const
	{
		return (x * x + y * y + z * z);
	}
	void norm()
	{
		float r = len();
		if(r > 0)
		{
			x /= r;
			y /= r;
			z /= r;
		}
	}
	vector3 normcopy()
	{
		float r = len();
		if(r > 0)
		{
			return vector3( this->x / r,
							this->y / r,
							this->z / r);
		}
		return vector3(0, 0, 0);
	}
	void fromanglelength(float _angle, float _r)
	{
		x = _r * cos(_angle);
		y = _r * sin(_angle);
	}
	float angle() const
	{
		return atan2(y, x);
	}
	float dot(const vector3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	vector3 cross(const vector3& v) const
	{
		vector3 n;
		n.x = - (y * v.z - z * v.y);
		n.y = - (z * v.x - x * v.z);
		n.z = - (x * v.y - y * v.x);
		return n;
	}
	void rot(float angle, const vector3& ax);
	vector3 rotcopy(float angle, const vector3& ax = vec::UY) const;

	static vector3 rnd(float min = 0, float max = 1);

	static vector3 rndrad(float r = 1);
};

const vector3 vector3::ZERO = vector3(0, 0, 0);
const vector3 vector3::UX = vector3(1, 0, 0);
const vector3 vector3::UY = vector3(0, 1, 0);
const vector3 vector3::UZ = vector3(0, 0, 1);
const vector3 vector3::CENTER = vector3(0.0, 0.0, MAXZ / 2);
real vector3::sEPSINON = EPSINON;
// ----------------------------------------
vector3 vector3::rnd(float min, float max)
{
	return vector3( rrnd(min, max), rrnd(min, max), rrnd(min, max) );
}
// ----------------------------------------
vector3 vector3::rndrad(float r)
{
	return rnd(-r, r).normcopy();
}

// **********************************************************************
// 四元数 
// **********************************************************************
struct  quaternion
{
	float w, x, y, z;

    quaternion (
        float fW = 1.0,
        float fX = 0.0, float fY = 0.0, float fZ = 0.0)
	{
		w = fW;
		x = fX;
		y = fY;
		z = fZ;
	}
    quaternion (const quaternion& rkQ)
	{
		w = rkQ.w;
		x = rkQ.x;
		y = rkQ.y;
		z = rkQ.z;
	}
	quaternion(const float& rfAngle, const vector3& rkAxis)
    {
        this->FromAngleAxis(rfAngle, rkAxis);
    }
    //-----------------------------------------------------------------------
    vector3 quaternion::operator* (const vector3& v) const
    {
		// nVidia SDK implementation
		vector3 uv, uuv; 
		vector3 qvec(x, y, z);
		uv = qvec.cross(v); 
		uuv = qvec.cross(uv); 
		uv = uv * (2.0f * w); 
		uuv = uuv * 2.0f; 
		
		return v + uv + uuv;
    }
	//-----------------------------------------------------------------------
    void quaternion::FromAngleAxis (float rfAngle,
        const vector3& rkAxis)
    {
        // assert:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        float fHalfAngle ( 0.5*rfAngle );
        float fSin = sin(fHalfAngle);
        w = cos(fHalfAngle);
        x = fSin*rkAxis.x;
        y = fSin*rkAxis.y;
        z = fSin*rkAxis.z;
    }
};
// ----------------------------------------
void vector3::rot(float angle, const vector3& ax)
{
	quaternion q(angle, ax);
	*this = q * (*this);
}
// ----------------------------------------
vector3 vector3::rotcopy(float angle, const vector3& ax) const
{
	quaternion q(angle, ax);
	return q * (*this);
}
// **********************************************************************
// ray3d
// **********************************************************************
struct ray3
{
	vector3 o;
	vector3 dir;
	ray3(){}
	ray3(const vector3& p1, const vector3& p2)
	{
		o = p1;
		dir = (p2 - p1).normcopy();
	}
	
};
// **********************************************************************
// segment
// **********************************************************************
struct segment
{
	vector3 s;
	vector3 e;
	segment(const vector3& _s, const vector3& _e)
	{
		s = _s;
		e = _e;
	}
};
// **********************************************************************
// plane
// **********************************************************************
struct plane
{
	vector3 o;
	vector3 n;
	plane(const vector3& _o, const vector3& _n)
	{
		o = _o;
		n = _n;
		n.norm();
	}
	plane(const vector3& p1, const vector3& p2, const vector3& p3)
	{
		n = (p2 - p1).cross(p3 - p1);
		n.norm();
		o = p1;
	}
};
// **********************************************************************
// boundingbox
// **********************************************************************
struct boundingbox
{
	vector3	a;
	vector3 b;
	boundingbox(){}
	boundingbox(const vector3& pt)
	{	
		a = pt;
		b = pt;
	}
	bool intersection(const vector3& pt)
	{		
		//MYTRACE("intersection " << a.y <<", "<< b.y);
		return 	pt.x > a.x && pt.x < b.x && 
				pt.y > a.y && pt.y < b.y && 
				pt.z > a.z && pt.z < b.z;
	}
	std::pair<bool, real> intersection(const ray3 &ray) const
	{
		const boundingbox& box = *this;
        real lowt = 0.0f;
        real t;
        bool hit = false;
        vector3 hitpoint;
        const vector3& min = box.a;
        const vector3& max = box.b;
        const vector3& rayorig = ray.o;
        const vector3& raydir = ray.dir;

        // Check origin inside first
        if ( rayorig > min && rayorig < max )
        {
            return std::pair<bool, real>(true, 0);
        }

        // Check each face in turn, only check closest 3
        // Min x
        if (rayorig.x < min.x && raydir.x > 0)
        {
            t = (min.x - rayorig.x) / raydir.x;
            if (t > 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max x
        if (rayorig.x > max.x && raydir.x < 0)
        {
            t = (max.x - rayorig.x) / raydir.x;
            if (t > 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Min y
        if (rayorig.y < min.y && raydir.y > 0)
        {
            t = (min.y - rayorig.y) / raydir.y;
            if (t > 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max y
        if (rayorig.y > max.y && raydir.y < 0)
        {
            t = (max.y - rayorig.y) / raydir.y;
            if (t > 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Min z
        if (rayorig.z < min.z && raydir.z > 0)
        {
            t = (min.z - rayorig.z) / raydir.z;
            if (t > 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max z
        if (rayorig.z > max.z && raydir.z < 0)
        {
            t = (max.z - rayorig.z) / raydir.z;
            if (t > 0)
            {
                // Substitute t back into ray and check bounds and dist
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }

        return std::pair<bool, real>(hit, lowt);
	}
	void centerzero(const vector3& pt)
	{
		a = pt;
		b = pt;		
	}
	void resize(const vector3& pt)
	{
		pt.x < a.x ? a.x = pt.x : 0;
		pt.x > b.x ? b.x = pt.x : 0;
		pt.y < a.y ? a.y = pt.y : 0;
		pt.y > b.y ? b.y = pt.y : 0;
		pt.z < a.z ? a.z = pt.z : 0;
		pt.z > b.z ? b.z = pt.z : 0;		
	}
	vector3 getcenter()
	{
		return (a + b) * 0.5;
	}
};

// -------------------------------------------------
#define MAXDEPTH 4
struct octree
{
	boundingbox aabb;
	octree*	children[8];
	std::vector<int> triinds;
	
	octree()
	{
		for(int i = 0; i < 8; i ++)
			children[i] = 0;
	}	
};

// -------------------------------------------------
octree* findnode(octree* oct, const vector3& p, int depth)
{
	for(int ii = 0; ii < 8; ii ++)
	{
		if(oct->children[ii]->aabb.intersection(p) )
		{
			if(depth >= MAXDEPTH - 1)
				return oct->children[ii];				
			return findnode(oct->children[ii], p, depth + 1);				
		}
	}
	return 0;
}
// -------------------------------------------------
octree* findnode(octree* oct, const ray3& ray, int depth)
{
	for(int ii = 0; ii < 8; ii ++)
	{
		if(oct->children[ii]->aabb.intersection(ray).first)
		{
			if(depth >= MAXDEPTH - 1)
				return oct->children[ii];				
			return findnode(oct->children[ii], ray, depth + 1);				
		}
	}
	return 0;
}
// -------------------------------------------------
void findnodes(octree* oct, const ray3& ray, int depth, std::vector<octree*>& result)
{
	for(int ii = 0; ii < 8; ii ++)
	{
		if(oct->children[ii]->aabb.intersection(ray).first)
		{
			if(depth >= MAXDEPTH - 1)		
				 result.push_back(oct->children[ii]);		
			else
				findnodes(oct->children[ii], ray, depth + 1, result);				
		}
	}
}

// **********************************************************************
// CTexture
// **********************************************************************
#define TEXSIZEX		1024
#define TEXSIZEY		1024
inline unsigned int widthByte32(unsigned int width, unsigned int depth)
{
	// 32 bits alignment (4 bytes)
	int rest=(width*depth/8)%4;
	if(rest != 0)
		return (width*depth/8 + 4-rest);
	else
		return (width*depth/8);
}

struct CTexture 
{
	unsigned char data[TEXSIZEX * TEXSIZEY * 3]; // 24位数据
	unsigned int at(int x, int y)
	{
		if( x >= TEXSIZEX  || x < 0 ||
			y >= TEXSIZEY  || y < 0 )
		return 0;

		int BytePerPixel = 24 / 8;
		int i = x, j = y;
		int m_WidthByte32 = widthByte32(TEXSIZEX, 24);
		return 
			(data[m_WidthByte32*j+i*BytePerPixel + 0]	<< 16) 	| 
			(data[m_WidthByte32*j+i*BytePerPixel + 1]   << 8 )  |	 
			 data[m_WidthByte32*j+i*BytePerPixel + 2]		   
			;	
	}
};
// **********************************************************************
// HSL
// **********************************************************************
typedef struct
{
    int hue;              // [0,255]
	int saturation;       // [0,255]
	int luminance;        // [0,255]
}COLOR_HSL;

// Converts HSL to RGB
int HSLtoRGB(const COLOR_HSL *hsl)
{
	float h = hsl->hue / 255.f * 360.f;     // h must be [0, 360]
    float s = hsl->saturation/255.f;		// s must be [0, 1]
	float l = hsl->luminance /255.f;        // l must be [0, 1]
    float R, G, B;
	if (hsl->saturation == 0)
	{
		// achromatic color (gray scale)
		R = G = B = l*255.f;
	}
	else
    {
        float q = (l<0.5f)?(l * (1.0f+s)):(l+s - (l*s));
        float p = (2.0f * l) - q;
        float Hk = h/360.0f;
        float T[3];
        T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0
        T[1] = Hk;              // Tb
        T[2] = Hk - 0.3333333f; // Tg
        for(int i=0; i<3; i++)
        {
            if(T[i] < 0) T[i] += 1.0f;
            if(T[i] > 1) T[i] -= 1.0f;
            if((T[i]*6) < 1)
            {
                T[i] = p + ((q-p)*6.0f*T[i]);
            }
            else if((T[i]*2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
            {
                T[i] = q;
            }
            else if((T[i]*3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
            {
                T[i] = p + (q-p) * ((2.0f/3.0f) - T[i]) * 6.0f;
            }
            else T[i] = p;
        }
        R = T[0]*255.0f;
        G = T[1]*255.0f;
        B = T[2]*255.0f;
    }
	unsigned char red = (unsigned char)((R>255) ? 255 : ((R<0) ? 0 : R));
	unsigned char green = (unsigned char)((G>255) ? 255 : ((G<0) ? 0 : G));
	unsigned char blue = (unsigned char)((B>255) ? 255 : ((B<0) ? 0 : B));
	return RGB(red, green, blue);
}

// **********************************************************************
// globles
// **********************************************************************
real				fmin1								= 0;
real				fmax1								= 0;

real				fheightmap0[IMAGESCALE][IMAGESCALE]	= {0};
real				fheightmap1[IMAGESCALE][IMAGESCALE]	= {0};
real				(*fheightmap)[IMAGESCALE]			= fheightmap0;

int					drawmap0[IMAGESCALE][IMAGESCALE]	= {0};
int					drawmap1[IMAGESCALE][IMAGESCALE]	= {0};
int					(*drawmap)[IMAGESCALE]				= drawmap0;

real				depthmap0[IMAGESCALE][IMAGESCALE]	= {0};
real				(*depthmap)[IMAGESCALE]				= depthmap0;

int					maskmap[IMAGESCALE][IMAGESCALE]		= {0};

real 				rndmap[512][512];

// **********************************************************************
// Perlin 2d random
// **********************************************************************
// Ken Perlin's Map Gennor
// ------------------------------------------------
int myfloor(real value){return (value >= 0 ? (int)value : (int)value - 1); } // 取整
inline real fade(real x){	return (x*x*x*(x*(6*x -15) + 10)); } // this equates to 6x^5 - 15x^4 + 10x^3  // 插值算法

// ------------------------------------------------
inline real dot(real x1, real y1, real x2, real y2)
{
	return x1 * x2 + y1 * y2;
}
// ------------------------------------------------
extern real blend(real h1, real h2, real alpha, real power);
void perlinmap(real map[IMAGESCALE][IMAGESCALE], int size, real* min1, real* max1, int octaves)
{
	//set up some variables
	int i,j,k,x,y,grad11,grad12,grad21,grad22;

	real pixel_value,
		  fracX, fracY,
		  noise11, noise12, noise21, noise22,
		  interpolatedx1, interpolatedx2, interpolatedxy,
		  amplitude, frequency,
		  gain = 0.50f, lacunarity = 2.0f;
	real gradients[8][2];
	int permutations[256];

	*min1 = 1000000;
	*max1 = -1000000;

	//梯度
	for (i = 0; i < 8; ++i)
	{
		gradients[i][0] = cos(0.785398163f * (real)i);
		gradients[i][1] = sin(0.785398163f * (real)i);
	}

	// 置换表
	{
		for (i = 0; i < 256; ++ i)
			permutations[i] = i;
		for (i = 0; i < 256; ++ i)
		{
			j = rand() % 256;
			k = permutations[i];
			permutations[i] = permutations[j];
			permutations[j] = k;
		}
	}

	// 生成MAP
	for (i = 0;	i < size; ++i)
	{
		for (j = 0;	j < size; ++j)
		{
			//get the value for this pixel by adding successive layers
			amplitude = 1.0f;
			frequency = 1.0f / (real)size;
			pixel_value = 0.0f;
			map[j][i] = 0;
			for (k = 0; k < octaves; ++k)
			{
				// 整数
				x = (int)((real)j * frequency);
				y = (int)((real)i * frequency);

				// 小数
				fracX = (real)j * frequency - (real)x;
				fracY = (real)i * frequency - (real)y;

				// 得到梯度索引
				grad11 = permutations[(x + permutations[y % 255]) % 255]			% 8;
				grad12 = permutations[(x + 1 + permutations[y % 255]) % 255]		% 8;
				grad21 = permutations[(x + permutations[(y + 1) % 255]) % 255]		% 8;
				grad22 = permutations[(x + 1 + permutations[(y + 1) % 255]) % 255]	% 8;

				// 四个角的梯度投影
				noise11 = dot(gradients[grad11][0], gradients[grad11][1], fracX, fracY);
				noise12 = dot(gradients[grad12][0], gradients[grad12][1], fracX - 1.0f, fracY);
				noise21 = dot(gradients[grad21][0], gradients[grad21][1], fracX, fracY - 1.0f);
				noise22 = dot(gradients[grad22][0], gradients[grad22][1], fracX - 1.0f, fracY - 1.0f);

				// 插值算法
				fracX = fade(fracX);
				fracY = fade(fracY);

				interpolatedx1 = blend(noise11, noise12, fracX, 1);
				interpolatedx2 = blend(noise21, noise22, fracX, 1);
				interpolatedxy = blend(interpolatedx1, interpolatedx2, fracY, 1);

				//	叠加
				pixel_value += interpolatedxy * amplitude;
				amplitude *= gain;

				// 缩小区域
				frequency *= lacunarity;
			}

			//put it in the map
			map[j][i] = pixel_value;

			//do some quick checks
			if (pixel_value < *min1)
				*min1 = pixel_value;
			else if (pixel_value > *max1)
				*max1 = pixel_value;
		}
	}
}
// ------------------------------------------------
// perlinnosie 3d
// ------------------------------------------------
struct ImprovedNoise 
{
   static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
   static double lerp(double t, double a, double b) { return a + t * (b - a); }
   static double grad(int hash, double x, double y, double z) {
      int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
      double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
             v = h<4 ? y : h==12||h==14 ? x : z;
      return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
   } 

   static double noise(double x, double y, double z) 
   {	 
		static int p[512] = {-1};
		if(p[0] == -1)
		{   
			static int permutation[512] = { 151,160,137,91,90,15,
			131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
			190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
			88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
			};

			for (int i=0; i <256; i++) p[256+i] = p[i] = permutation[i]; 
		}
		
		
		int X = (int)floor(x) & 255,                  	// FIND UNIT CUBE THAT
		Y = (int)floor(y) & 255,                  		// CONTAINS POINT.
		Z = (int)floor(z) & 255;
		x -= floor(x);                                	// FIND RELATIVE X,Y,Z
		y -= floor(y);                                	// OF POINT IN CUBE.
		z -= floor(z);
		double u = fade(x),                              // COMPUTE FADE CURVES
			 v = fade(y),                                // FOR EACH OF X,Y,Z.
			 w = fade(z);
		int A = p[X]+Y, AA = p[A]+Z, AB = p[A+1]+Z,      // HASH COORDINATES OF
		  B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,

		return lerp(w, lerp(v, 	lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
										grad(p[BA  ], x-1, y  , z   )), // BLENDED
									lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
										 grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
									lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
										 grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
									lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
										 grad(p[BB+1], x-1, y-1, z-1 ))));
   }   
};
real perlin(real x, real y, real z) 
{
	return ImprovedNoise::noise(x, y, z);
}

double fBm(double x, double y, double z, int octaves)
{
    double frequency = 1.0f;
    double amplitude = 0.5f;
    double sum = 0.0f;
	double gain = 0.50f;
	double lacunarity = 2.0f;
	
    for (int i = 0; i < octaves; i++)
    {
        sum += perlin(x * frequency, y * frequency, z * frequency) * amplitude;
		//sum += amplitude * (1-abs(perlin(x * frequency, y * frequency, z * frequency)));
        frequency *= lacunarity;
        amplitude *= gain;
    }
    return sum;
}
inline double ridge(double h, float offset)
{
	h = abs(h);
	h = offset - h;
	h = h * h;
	return h;
}
double RidgedMF(double x, double y, double z, int octaves, float offset)
{
	double sum = 0;
	float amplitude = 0.5f;
	float frequency = 1.0f;
	double prev = 1.0f;
	
	double gain = 0.50f;
	double lacunarity = 2.5f;
	
	for (int i = 0; i < octaves; i++)
	{
		double n = ridge(perlin(x * frequency, y * frequency, z * frequency), offset);		
		sum += n * amplitude * prev;
		prev = n;
		frequency *= lacunarity;
		amplitude *= gain;
	}

	return sum;
}


// **********************************************************************
// shape blend
// **********************************************************************
real blend(real h1, real h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if(power != 1.0)
		alpha = pow(alpha, power);

	return h1 * (1.0 - alpha) + h2 * alpha;
}
vector2 blend(vector2 v1, vector2 v2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if(power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}
vector3 blend(vector3 v1, vector3 v2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	if(power != 1.0)
		alpha = pow(alpha, power);

	return v1 * (1 - alpha) + v2 * alpha;
}
// ------------------------------------------------
real blend2(real h1, real h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if(alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if(power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if(power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}
// ------------------------------------------------
vector2 blend2(vector2 h1, vector2 h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if(alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if(power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if(power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}
// ------------------------------------------------
vector3 blend2(vector3 h1, vector3 h2, real alpha, real power = 1.0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if(alpha < 0.5f)
	{
		alpha = (0.5f - alpha) * 2.0f;
		if(power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5f) * 2.0f;
		if(power != 1.0)
			alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}

float blend3(float h1, float h2, float alpha, float mid = 0.5, float power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if(alpha < mid)
	{
		alpha = (mid - alpha) / mid;
		alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;	}
	else
	{
		alpha = (alpha - mid) / (1 - mid);
		alpha = pow(alpha, power);
		return h2 * (1 - alpha) + h1 * alpha;
	}
}

float blend4(float h1, float h2, float alpha, float power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	float mid = 0.5;
	if(alpha < mid)
	{
		alpha = alpha / mid;
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (1 - alpha) / (1 - mid);
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
}

float blend5(float h1, float h2, float alpha, float mid = 0.5, float power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	if(alpha < mid)
	{
		alpha = alpha / mid;
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (1 - alpha) / (1 - mid);
		alpha = pow(alpha, 1.0f / power);
		return h1 * (1 - alpha) + h2 * alpha;
	}
}

// ------------------------------------------------------------------------------------------------
vec3 bezier2(vec3 cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
vec3 bezier3(vec3 cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) 	+
			cp[1] * (3 * t * s * s) +
			cp[2] * (3 * t * t * s) +
			cp[3] * (t * t * t);
}
// ------------------------------------------------------------------------------------------------
vec2 bezier2(vec2 cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
vec2 bezier3(vec2 cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) +
		cp[1] * (3 * t * s * s) +
		cp[2] * (3 * t * t * s) +
		cp[3] * (t * t * t);
}
// ------------------------------------------------------------------------------------------------
real bezier2(real cp[3], real t)
{
	return cp[0] * ((1 - t) * (1 - t)) + cp[1] * (2 * t * (1 - t)) + cp[2] * (t * t);
}
// ------------------------------------------------------------------------------------------------
real bezier3(real cp[4], real t)
{
	real s = 1 - t;
	return  cp[0] * (s * s * s) +
		cp[1] * (3 * t * s * s) +
		cp[2] * (3 * t * t * s) +
		cp[3] * (t * t * t);
}
// -----------------------------------------------------------------------
inline float roundblend(float h1, float h2, float alpha)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;
	alpha = sqrt(1 - (1-alpha)*(1-alpha));

	return h1 * (1 - alpha) + h2 * alpha;
}
// -----------------------------------------------------------------------
inline float roundblend2(float h1, float h2, float alpha, float power = 1)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	alpha = pow(alpha, power);

	if(alpha < 0.5)
	{
		alpha = alpha * 2;
		alpha = sqrt(1 - (1-alpha)*(1-alpha));

		return h1 * (1 - alpha) + h2 * alpha;
	}
	else
	{
		alpha = (alpha - 0.5) * 2;
		alpha = sqrt(1 - (alpha)*(alpha));

		return h1 * (1 - alpha) + h2 * alpha;
	}

}

// -----------------------------------------------------------------------
// 三角函数插值
inline float BlendSin(float h1, float h2, float alpha)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	alpha = sin(alpha * PI / 2);

	return h1 * (1 - alpha) + h2 * alpha;
}
// -----------------------------------------------------------------------
// 傅立叶级数
inline float FT(float angle, float t[] = 0, float dt = 0)
{
	if(t == 0)
	{
		static float s_t0[] = {rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1)};
		t = s_t0;
	}

	float yy = 0;
	yy += 1		* sin(1 * angle + (t[0] + dt) * PI);
	yy += 0.5	* sin(2 * angle + (t[1] + dt) * PI);
	yy += 0.25	* sin(4 * angle + (t[2] + dt) * PI);
	yy += 0.125	* sin(8 * angle + (t[3] + dt) * PI);

	return yy;
}
// -----------------------------------------------------------------------
inline float FTU(float ang, float t[] = 0, float dt = 0)
{
	float ft = FT(ang, t, dt);
	float max1 = (1 + 0.5 + 0.25 + 0.125);
	float min1 = - max1;
	return (ft - min1) / (max1 - min1);
}
// -----------------------------------------------------------------------
inline float BlendFT(float h1, float h2, float alpha, float t[] = 0, float dt = 0)
{
	alpha = alpha < 0 ? 0 : alpha;
	alpha = alpha > 1 ? 1 : alpha;

	alpha = FTU(alpha, t, dt);

	return h1 * (1 - alpha) + h2 * alpha;
}
// -----------------------------------------------------------------------
inline float FT2D1(float anglex, float angley, float tx[] = 0, float ty[] = 0, float dtx = 0, float dty = 0)
{
	if (tx == 0)
	{
		static float s_t0x[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		tx = s_t0x;
	}
	if (ty == 0)
	{
		static float s_t0y[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		ty = s_t0y;
	}

	float yy = 0;
	yy += 1		* sin(1 * anglex + (tx[0] + dtx) * PI) + 1	   * sin(1 * angley + (ty[0] + dty) * PI);
	yy += 0.5	* sin(2 * anglex + (tx[1] + dtx) * PI) + 0.5   * sin(2 * angley + (ty[1] + dty) * PI);
	yy += 0.25  * sin(4 * anglex + (tx[2] + dtx) * PI) + 0.25  * sin(4 * angley + (ty[2] + dty) * PI);
	yy += 0.125 * sin(8 * anglex + (tx[3] + dtx) * PI) + 0.125 * sin(8 * angley + (ty[3] + dty) * PI);

	return yy;
}
// ------------------------------------------------------------------------------------------------
real FFT(real ax, real ay, real rndmap[512][512])
{
	real ft = 0;
	for(int i = 0; i < 8; i ++)
	{
		real dz = blend(ax, ay, rndmap[100][i], 2);
		{
			real A  = 2 * rndmap[0][i];
			real F  = blend(20, 50, rndmap[1][i]);
			real T0 = 2 * PI * rndmap[2][i];
			ft += A * (0.5 + 0.5 * sin(T0 + F * dz * PI * 2));
		}
	}
	return ft / (8);
}

// -----------------------------------------------------------------------
inline float FT2D(float anglex, float angley, float tx[] = 0, float ty[] = 0, float dtx = 0, float dty = 0)
{
	static float s_fx[] = { rrnd(0, 8), rrnd(0, 4), rrnd(0, 2), rrnd(0, 1) };
	static float s_fy[] = { rrnd(0, 8), rrnd(0, 4), rrnd(0, 2), rrnd(0, 1) };
	if (tx == 0)
	{
		static float s_t0x[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		tx = s_t0x;
	}
	if (ty == 0)
	{
		static float s_t0y[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		ty = s_t0y;
	}

	float yy = 0;
	yy += 1 		* sin(s_fx[0] * anglex + (tx[0] + dtx) * PI + s_fy[0] * angley + (ty[0] + dty) * PI);
	yy += .5 		* sin(s_fx[1] * anglex + (tx[1] + dtx) * PI + s_fy[1] * angley + (ty[1] + dty) * PI);
	yy += .25 	* sin(s_fx[2] * anglex + (tx[2] + dtx) * PI + s_fy[2] * angley + (ty[2] + dty) * PI);
	yy += .125 	* sin(s_fx[3] * anglex + (tx[3] + dtx) * PI + s_fy[3] * angley + (ty[3] + dty) * PI);

	return yy;
}

// ------------------------------------------------
// 2d
real blend2d(real h1, real h2, real alphaX, real alphaY)
{
	int size;
	real alpha;
	alphaX < 0 ? alphaX = 0 : 0;
	alphaX > 1 ? alphaX = 1 : 0;
	alphaY < 0 ? alphaY = 0 : 0;
	alphaY > 1 ? alphaY = 1 : 0;
	size = IMAGESCALE;
	alpha = fheightmap[(int)(alphaX * size + 0.5) % size][(int)(alphaY * size + 0.5) % size];
	alpha = (alpha - fmin1) / (fmax1 - fmin1);

	return h1 * (1 - alpha) + h2 * alpha;
}
// **********************************************************************
// color blend
// **********************************************************************
int blendcor(int c1, int c2, real alpha, real power = 1.0)
{
	//alpha = alpha != 1 ? abs(alpha) - (int)(alpha) : 1;	
	alpha = alpha > 1 ? 1 : alpha;
	alpha = alpha < 0 ? 0 : alpha;
	
	if(power != 1.0)
		alpha = pow(alpha, power);
	 return RGB( GetRValue(c2) * alpha + GetRValue(c1) * (1 - alpha),
				 GetGValue(c2) * alpha + GetGValue(c1) * (1 - alpha),
				 GetBValue(c2) * alpha + GetBValue(c1) * (1 - alpha)
				 );
	
}

// ------------------------------------------------
// 2d
int blendcor2d(int c1, int c2, real alphaX, real alphaY, real power = 1.0)
{
	real alpha;
	int size = IMAGESCALE;
	alphaX < 0 ? alphaX = 0 : 0;
	alphaX > 1 ? alphaX = 1 : 0;
	alphaY < 0 ? alphaY = 0 : 0;
	alphaY > 1 ? alphaY = 1 : 0;
	
	alpha = fheightmap[(int)(alphaX * size) % size][(int)(alphaY * size) % size];
	alpha = (alpha - fmin1) / (fmax1 - fmin1);

	return blendcor(c1, c2, alpha, power);
}
// **********************************************************************
// color add
// **********************************************************************
inline int addcor(int c1, int c2)
{
	int r = GetRValue(c1) + GetRValue(c2);
	int g = GetGValue(c1) + GetGValue(c2);
	int b = GetBValue(c1) + GetBValue(c2);
	 return RGB( r > 255 ? 255 : r,
				 g > 255 ? 255 : g,
				 b > 255 ? 255 : b );
	
}
// **********************************************************************
// 3d数学
// **********************************************************************
// cross
void cross(crvec v1, crvec v2, rvec dest)
{
    dest.x=v1.y*v2.z-v1.z*v2.y;
    dest.y=v1.z*v2.x-v1.x*v2.z;
    dest.z=v1.x*v2.y-v1.y*v2.x;
}
/**
	计算一条3d空间直线和一个3d空间平面的交点
	@param            pnv        空间平面的法线的单位矢量
	@param            pd        空间平面的参数距离,平面的方程为: pnv.dot( pv ) = pd
	@param            ltv        空间直线的方向(切向)单位矢量
	@param            lv        空间直线上的一点
	@param            outV    计算后得到的位于平面上的交点坐标
	
*/
void planeAndSLIntersectionV(crvec pnv, real pd, crvec ltv, crvec lv, rvec outV)
{
	real dis = pnv.dot( lv ) - pd;
	
	outV.x = ltv.x * 1000 + lv.x;
	outV.y = ltv.y * 1000 + lv.y;
	outV.z = ltv.z * 1000 + lv.z;
	
	real td = pnv.dot( outV ) - pd;
	
	real k = dis / (dis - td);
	
	outV.x -= lv.x;
	outV.y -= lv.y;
	outV.z -= lv.z;
	
	outV.x *= k;
	outV.y *= k;
	outV.z *= k;
	
	outV.x += lv.x;
	outV.y += lv.y;
	outV.z += lv.z;
}
/**
* 计算3d空间两个平面相交直线,得到的直线数据为: 直线的切向ltv和直线上的一点lpv
* @param  panv  空间平面a的法线的单位矢量
  @param  pad          空间平面a的参数距离,平面的方程为: pnv.dot( pv ) = pd
  @param  pbnv  空间平面b的法线的单位矢量
  @param  pbd          空间平面b的参数距离,平面的方程为: pnv.dot( pv ) = pd
  @param  ltv          两个平面交线的切向
  @param  lv          两个平面交线上的一点
* 
**/

void pAndPIntersectionLine(crvec panv, real pad, crvec pbnv, real pbd, rvec ltv, rvec lv)
{
	// 计算出两条法线的叉积,即为交线的切向
	cross(panv, pbnv, ltv);
	ltv.norm();
	// 计算平面a上的一点
	vec pav;
	pav.x = pad * panv.x;
	pav.y = pad * panv.y;
	pav.z = pad * panv.z;
	vec tnv;
	// 计算出在平面a上的,垂直于交线ltv的直线的切向
	cross(ltv, panv, tnv);
	// 计算由pav和tnv决定的直线和平面b的交点,
	// 这个交点就是两个平面交线上的一点
	planeAndSLIntersectionV(pbnv, pbd, tnv, pav, lv);
                        
}
// ------------------------------------------------
void getplanepm(crvec p1, crvec p2, crvec p3, rvec n, real& d)
{
	n = (p2 - p1).cross(p3 - p1); n.norm();
	d = p1.dot(n);
}

// ------------------------------------------------
// A = y2 - y1,B = x1- x2,C = x2 * y1 - x1 * y2
//distance(P,l)=|aX+bY+c|/   
inline real distance(const vector3& p1, const vector3& p2)
{
	return (p1 - p2).len();
}
//点到线段的最短距离,x0,y0是圆心
real distance(vector3 p, vector3 p1,vector3 p2)
{
    double ans = 0;
    double a, b, c;
    a = distance(p1, p2);
    b = distance(p1, p);
    c = distance(p2, p);
    if (c+b==a) {//点在线段上
      ans = 0;
      return ans;
    }
    if (a<=0.00001) {//不是线段，是一个点
      ans = b;
      return ans;
    }
    if (c*c >= a*a + b*b) { //组成直角三角形或钝角三角形，p1为直角或钝角
      ans = b;
      return ans;
    }
    if (b * b >= a * a + c * c) {// 组成直角三角形或钝角三角形，p2为直角或钝角
      ans = c;
      return ans;
    }
    // 组成锐角三角形，则求三角形的高
    double p0 = (a + b + c) / 2;// 半周长
    double s = sqrt(p0 * (p0 - a) * (p0 - b) * (p0 - c));// 海伦公式求面积
    ans = 2*s / a;// 返回点到线的距离（利用三角形面积公式求高）
    return ans;
}

real distance(const vector3& p, const segment& l)
{	
	return distance(p, l.s, l.e);
}
real distance(const vector3& p1s, const vector3& p1e,const vector3& p2s, const vector3& p2e)
{	
	segment l1(p1s, p1e);
	segment l2(p2s, p2e);
	
	return MIN(
		MIN( distance(p1s,l2),distance(p1e,l2) ),	
		MIN( distance(p2s,l1),distance(p2e,l1) )
	);
}
// -----------------------------------------------------------------
// 三角形碰撞 
bool  checkPointInTriangle(
	const vector3& point, 
	const vector3& a, const vector3& b, const vector3& c,
	float tolerance = 0.005f) 
{		
	real total_angles = 0.0f;	

	// make the 3 vectors

	vector3 v1(point.x - a.x, point.y - a.y, point.z - a.z);
	v1.norm();
	vector3 v2(point.x - b.x, point.y - b.y, point.z - b.z);
	v2.norm();
	vector3 v3(point.x - c.x, point.y - c.y, point.z - c.z);	
	v3.norm();
	
	real Dot1 = v2.dot(v1);
	Dot1 < -1.0f ?	 Dot1 = -1.0f : NULL;
	Dot1 >  1.0f ?   Dot1 =  1.0f : NULL;
	total_angles += acos(Dot1); 

	real Dot2 = v3.dot(v2);
	Dot2 < -1.0f ? 	Dot2 = -1.0f : NULL;
	Dot2 >  1.0f ?  Dot2 =  1.0f : NULL;
	total_angles += acos(Dot2);

	real Dot3 = v1.dot(v3);
	Dot3 < -1.0f ?	Dot3 = -1.0f : NULL;
	Dot3 >  1.0f ?	Dot3 =  1.0f : NULL;
	total_angles += acos(Dot3); 
			
	if (fabs(total_angles - 2.0f * PI) <= tolerance)			
		return true;

	return false;
}


// **********************************************************************
// projection
// **********************************************************************
inline vector2 viewprjold(const vector3& p)
{
	real vpf = blend(1, 0.1, p.z / 16.0);
	return vector2(
		0.5 + (p.x - 0.5) * vpf,
		0.95 + (p.y - 0.95) * vpf
		);		
}
extern inline void viewprj(const vector3& rp, vector2& p2d, real& depth);
// **********************************************************************
// common draw
// **********************************************************************
// pixel
inline bool pixel(int ix, int iy , int color, real alpha = 1, real depth = 0)
{
	if(ix < 0 || iy < 0 || ix >= IMAGESCALE || iy >= IMAGESCALE)
		return false;
	if (depth >= 0 && 
	   (depthmap[ix][iy] == 0 || depth <= depthmap[ix][iy])
	 )
	{
		if (alpha >= 1)
		{
			drawmap[ix][iy] = color;
			depthmap[ix][iy] = depth;// < 0 ? 0 : depth;
		}
		else
		{
			drawmap[ix][iy] = blendcor(drawmap[ix][iy], color, alpha, 1);
			//depthmap[ix][iy] = depth;// < 0 ? 0 : depth;
		}
		return true;
	}
	
	return false;
}
inline bool pixel(real x, real y, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	return pixel(int(x * IMAGESCALE + 0.5), int(y * IMAGESCALE + 0.5), color, alpha, depth);
}
inline bool pixel(const vector2& p, int color = 0xFFFFFFFF, real alpha = 1, real depth = 0)
{
	return pixel(int(p.x * IMAGESCALE + 0.5), int(p.y * IMAGESCALE + 0.5), color, alpha, depth);
}
inline bool pixel(const vector3& p, int color = 0xFFFFFFFF, real alpha = 1.0)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	return pixel(p2d, color, alpha, depth);
}
// 抗锯齿算法
inline bool pixelAA(real x, real y, int color = 0xFFFFFFFF, real depth = 0)
{
	x = x * IMAGESCALE;
	y = y * IMAGESCALE;
	int ix = int(x);
	int iy = int(y);
	real fx = x - ix;
	real fy = y - iy;

	if (ix < 0 || iy < 0 || ix >= IMAGESCALE - 1 || iy >= IMAGESCALE - 1)
		return false;
	real& rdepthval = depthmap[int(x + 0.5)][int(y + 0.5)];
	if(depth >= 0 && 	
	  (rdepthval == 0 || depth <= rdepthval)
	)
	{		
		drawmap[ix][iy] 		= blendcor(drawmap[ix][iy], color, (1 - fx) * (1 - fy));	
		drawmap[ix + 1][iy] 	= blendcor(drawmap[ix + 1][iy], color, (fx) * (1 - fy));		
		drawmap[ix][iy + 1] 	= blendcor(drawmap[ix][iy + 1], color, (1 - fx) * (fy));	
		drawmap[ix + 1][iy + 1] = blendcor(drawmap[ix + 1][iy + 1], color, (fx) * (fy));		
		
		drawmap[int(x + 0.5)][int(y + 0.5)] = color;
		rdepthval = depth < 0 ? 0 : depth;		
		return true;
	}	
	return false;
}
inline bool pixelAA(const vector2& p, int color, real depth = 0)
{
	return pixelAA(p.x, p.y, color, depth);
}
inline bool pixelAA(const vector3& p, int color)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	return pixelAA(p2d, color, depth);
}

// point
inline void pointf(int x0, int y0, int size, int color, real alpha = 1, real depth = 0)
{
	if (size == 0)
		return;

	//if (!pixel(x0, y0, color, 1, depth))
	//	return;

	if (size == 1)
		return;

	int halfsize = size / 2;
	int x1 = (x0 - halfsize); x1 < 0 ? x1 = 0 : 0;
	int y1 = (y0 - halfsize); y1 < 0 ? y1 = 0 : 0;
	int x2 = (x0 + halfsize); x2 >= IMAGESCALE ? x2 = IMAGESCALE - 1 : 0;
	int y2 = (y0 + halfsize); y2 >= IMAGESCALE ? y2 = IMAGESCALE - 1 : 0;

	for (int i = x1; i < x2; i++)
	for (int j = y1; j < y2; j++)
	{
		real alpha1 = blend2d(0.5, 1, real(i) / IMAGESCALE, real(j) / IMAGESCALE);

		real sqrdis = (i - x0)*(i - x0) + (j - y0)*(j - y0);
		real dhalfsize = blend(0, halfsize * halfsize, alpha1, alpha1 * 4);
		real ds = (1 - sqrdis / dhalfsize);
		if (ds > 0)
		{
			if (depthmap[i][j] == 0 || depth <= depthmap[i][j])
				drawmap[i][j] = blendcor(drawmap[i][j], color, alpha);
		}
	}
}
inline void point(real x0, real y0, int size, int color, real alpha = 1, real depth = 0)
{
	pointf((x0 * IMAGESCALE), (y0 * IMAGESCALE), (size), color, alpha, depth);
}
inline void point(const vector2& p, int size, int color, real alpha = 1, real depth = 0)
{
	point(p.x, p.y, size, color, alpha, depth);
}
inline void point(const vector3& p, int size, int color, real alpha = 1)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);	
	point(p2d, size, color, alpha, depth);
}

inline void pointf_sharp(int x0, int y0, int size, int color, real depth = 0)
{
	if (size == 0)
		return;

	if (!pixel(x0, y0, color, 1, depth))
		return;

	if (size == 1)
		return;

	int halfsize = size / 2;
	int x1 = (x0 - halfsize); x1 < 0 ? x1 = 0 : 0;
	int y1 = (y0 - halfsize); y1 < 0 ? y1 = 0 : 0;
	int x2 = (x0 + halfsize); x2 >= IMAGESCALE ? x2 = IMAGESCALE - 1 : 0;
	int y2 = (y0 + halfsize); y2 >= IMAGESCALE ? y2 = IMAGESCALE - 1 : 0;

	for (int i = x1; i < x2; i++)
	for (int j = y1; j < y2; j++)
	{
		real sqrdis = (i - x0)*(i - x0) + (j - y0)*(j - y0);		
		real ds = (1 - sqrdis / (halfsize * halfsize));
		if (ds > 0)
		{
			if (depthmap[i][j] == 0 || depth <= depthmap[i][j])
			{
				drawmap[i][j] = color;// blendcor(drawmap[i][j], color, ds, 0.25 + (1 - ds) * 2);
				depthmap[i][j] = depth;
			}
		}
	}
}
inline void point_sharp(real x0, real y0, int size, int color, real depth = 0)
{
	pointf_sharp(int(x0 * IMAGESCALE + 0.5), int(y0 * IMAGESCALE + 0.5), size, color, depth);
}
inline void point_sharp(const vector2& p, int size, int color, real depth = 0)
{
	point_sharp(p.x, p.y, size, color, depth);
}
inline void point_sharp(const vector3& p, int size, int color)
{
	vec2 p2d;
	real depth;
	viewprj(p, p2d, depth);
	point_sharp(p2d, size, color, depth);
}
extern real getviewdis(const vec& p1, const vec& p2);
// line
void line3d(vec p1, vec p2, int cor = 0xFFFFFFFF)
{
	int len = getviewdis(p1, p2);
	for(int i = 0; i <= len; i ++)
	{
		real ai = i / real(len);
		pixel(blend(p1, p2, ai), cor);
	}
}
void line2d(vec2 p1, vec2 p2, int cor = 0xFFFFFFFF)
{
	int len = (p2 - p1).len() * IMAGESCALE;
	for(int i = 0; i <= len; i ++)
	{
		real ai = i / real(len);
		pixel(blend(p1, p2, ai), cor);
	}
}

// **********************************************************************
// read/write BMP
// **********************************************************************
int bmp_read(unsigned char *image, const char *filename)
{	
	FILE *fp;
	unsigned char header[54];

	if (0 != (fopen_s(&fp, filename, "rb")))
	{
		PRINT("file not exist!\n");	
		return -1;
	}
	       
	fread(header, sizeof(unsigned char), 54, fp);
	fread(image, sizeof(unsigned char), (size_t)(long)2048 * 2048 * 3, fp);
	     
	fclose(fp);
	return 0;
}
int bmp_write( unsigned char* image, int xSize, int ySize, char *fileName)  
{  
	unsigned char header[54] = {
		0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0
	};
          
	// sizeof(long) = 4  
	long fileSize = (long)xSize * (long)ySize * 3 +54 ;     // image array size + 54  
	long temp = 0;                      // temp for byte convertion  
	long width = xSize;                 // image width  
	long height = ySize;                // image height 
	int i = 0;                          // loop variable  

	FILE *fp;  

	for( i=0 ; i != 4 ; ++i)                // write fileSize from byte2 ~ byte 6  
	{  
		header[2+i] = (unsigned char)( fileSize >> (8*i) ) & 0x000000ff;        
	}  
	for( i=0 ; i != 4 ; ++i)                // write width from byte18 ~ byte 22  
	{  
		header[18+i] = (unsigned char)( width >> (8*i) ) & 0x000000ff;          
	}  
	for( i=0 ; i != 4 ; ++i)                // write height from byte22 ~ byte 26  
	{  
		header[22+i] = (unsigned char)( height >> (8*i) ) & 0x000000ff;         
	}


	if (fopen_s(&fp, fileName, "w") != 0)
	{
		PRINT("bmp_write openfile " << fileName << " failed!")
		return -1;
	}

	fwrite(header, sizeof(unsigned char), 54, fp);                              // write header  
	fwrite(image, sizeof(unsigned char),(size_t)(long)xSize*ySize*3, fp);       // write image array  

	fclose(fp);  
	return 0;  
} 
// **********************************************************************
// 几何体造型基础
// **********************************************************************
struct vertex
{
	vec p;
	int cor;
	vec2 uv;
	vertex()
	{
		cor = 0;
	}
	vertex(const vec& _p)
	{
		p = _p;
		cor = 0;		
	}
	operator const vec&()
	{
		return p;
	}
	bool operator==(const vertex& v) const
	{
		return p == v.p;
	}	
	vec operator + (const vertex& v) const
	{
		return p + v.p;
	}
	vec operator - (const vertex& v) const
	{
		return p - v.p;
	}
	vec operator * (real s) const
	{
		return p * s;
	}
};
struct triangle
{	
	int	vertexIndex[3];
	int	normalIndex[3];
	int	uvIndex[3];
	int vercor[3];
	bool enable;
	triangle() : enable(1){}
};
struct submesh
{
	std::vector<vector3> vertices;
	std::vector<int> cors;
	std::vector<vector3> normals;
	std::vector<vector2> uvs;	
	std::vector<triangle> tris;	
};

// -------------------------------------------------
inline void drawline(vec3 p1, vec3 p2)
{
	for(int i = 0; i < 1000; i ++)
	{
		vec3 p = blend(p1, p2, i / 1000.0);
		vec2 p2d;
		real depth;
		viewprj(p, p2d, depth);
		pixel(p2d, 1, depth);
	}
}

// ------------------------------------------------------------------------------------------------
extern int renderstate;
void drawtriangle(const vec& p1, const vec& p2, const vec& p3, int cor = 1)
{
	if(renderstate == 1)
	{
		static vec light(0.25, 1, -1);	
		vec norm = (p2 - p1).cross(p3 - p1);
		norm = norm.normcopy();
		if(norm.sqrlen() == 0)
			return; // 无效三角形	

		int cor0 = rrnd(50, 80);//0x000000FF & cor;
		{
			int lum = blend(cor0, 255, norm.dot(light));
			cor = RGB(lum, lum, lum);			
		}			
		
		LOOP(i, len, getviewdis(p1, p2))		
			vec p12 = blend(p1, p2, ai);	
			LOOP(j, wid, getviewdis(p12, p3))			
				vec p = blend(p12, p3, aj);
				pixelAA(p, cor);	
			END
		END
	}
	//else if(renderstate == 2)
	{
		line3d(p1, p2, 0xFFFFFFFF);
		line3d(p2, p3, 0xFFFFFFFF);
		line3d(p3, p1, 0xFFFFFFFF);
	}	
}
// -------------------------------------------------
void rendersubmesh(const submesh& sm)
{
	const std::vector<vector3>& vertices = sm.vertices;
	const std::vector<triangle>& tris = sm.tris;	
	
	for(int i = 0; i < tris.size(); i ++)
	{
		const triangle& tri = tris[i];
		if(!tri.enable)
			continue;
		vec pos[3];
		for(int ii = 0; ii < 3; ii ++)
		{	
			int ind = tri.vertexIndex[ii];		
			pos[ii] = vertices[ tri.vertexIndex[ii] ];		
		}
		drawtriangle(pos[0], pos[2], pos[1]);
	}
}	

// -------------------------------------------------
bool loadsubmesh(const char* fileName, submesh& sm)
{	
	FILE * file;
	if (0 != (fopen_s(&file, fileName, "r")))
	{
		printf("File can't be opened\n");
		return false;
	}

	while( 1 ){
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf_s(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 )
		{		
			vector3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			sm.vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{
			vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			sm.uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{			
			vector3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			sm.normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			triangle tri;	
			int matches = fscanf_s(file, "%d//%d %d//%d %d//%d\n", &tri.vertexIndex[0], &tri.normalIndex[0], &tri.vertexIndex[1], &tri.normalIndex[1], &tri.vertexIndex[2], &tri.normalIndex[2]);
			if (matches != 6)
			//int matches = fscanf(file, "%d//%d//%d %d//%d//%d %d//%d//%d\n", &tri.vertexIndex[0], &tri.normalIndex[0], &tri.uvIndex[0], &tri.vertexIndex[1], &tri.normalIndex[1], &tri.uvIndex[1], &tri.vertexIndex[2], &tri.normalIndex[2], &tri.uvIndex[2]);
			//if (matches != 9)			
			{
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			//PRINT(tri.vertexIndex[2])
			sm.tris.push_back(tri);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}	

	fclose(file);
	
	return true;	
}


// ------------------------------------------------
#define SMCATCH_START(wid)	submesh sm;	triangle tri1, tri2; int verIndex = 0;int lowerind[wid + 1];	
#define SMCATCH_ING(wid, s, i, j, pp) 	{const int step = s;	if ((i % step == 0 && j % step == 0)){if(i > 0 && j > 0){tri1.vertexIndex[2] = lowerind[j - step];tri1.vertexIndex[1] = lowerind[j];tri1.vertexIndex[0] = verIndex - 1;tri2.vertexIndex[2] = verIndex;tri2.vertexIndex[1] = verIndex - 1;	tri2.vertexIndex[0] = lowerind[j];sm.tris.push_back(tri1);sm.tris.push_back(tri2);}	sm.vertices.push_back(pp);lowerind[j - step] = verIndex - 1;if(j == wid) lowerind[j] = verIndex;verIndex ++;}}
#define SMCATCH_ING2(wid, s, i, j, pp) 	{const int step = s;	if ((i % step == 0 && j % step == 0)){if(i > 0 && j > 0){tri1.vertexIndex[0] = lowerind[j - step];tri1.vertexIndex[1] = lowerind[j];tri1.vertexIndex[2] = verIndex - 1;tri2.vertexIndex[0] = verIndex;tri2.vertexIndex[1] = verIndex - 1;	tri2.vertexIndex[2] = lowerind[j];sm.tris.push_back(tri1);sm.tris.push_back(tri2);}	sm.vertices.push_back(pp);lowerind[j - step] = verIndex - 1;if(j == wid) lowerind[j] = verIndex;verIndex ++;}}

// ------------------------------------------------
std::string filename = "E:/U3DC#/VArchitecture/md.txt";
void exportdat(const submesh& sm)
{
	{// 3d data
		FILE* file;
		if (0 != fopen_s(&file, filename.c_str(), "w"))
			return;
		
		fprintf(file, "//----- vertices %d ------\n", sm.vertices.size());
		fprintf(file, "float[,] vertices = {\n");		
		for(int i = 0; i < sm.vertices.size(); i ++)
		{
			const vec& v = sm.vertices[i];
			fprintf(file, "{%ff, %ff, %ff},\n", v.x, v.y, v.z);			
		}
		fprintf(file, "};\n");
		
		fprintf(file, "//----- triangles %d------\n", sm.tris.size());
		fprintf(file, "int[] triangles = new int [] {\n");	
		for(int i = 0; i < sm.tris.size(); i ++)
		{			
			const triangle& tri = sm.tris[i];
			fprintf(file, "%d, %d, %d,\n", tri.vertexIndex[0], tri.vertexIndex[1], tri.vertexIndex[2]);		
		}
		fprintf(file, "};\n");
		fclose(file);
	}
}
// ------------------------------------------------
// log
// ------------------------------------------------
void savelog(const std::string& log)
{
	if (!gblog)
		return;
	std::string logfilename = "E:\\VGarden\\Building\\OUTPUT\\log.txt";

	FILE* file;
	if (0 != fopen_s(&file, logfilename.c_str(), "w"))
		return;

	fprintf(file, log.c_str());		
	fclose(file);
	//logmessage.str("");
}
// **********************************************************************
// 三角形导出
// **********************************************************************
submesh gsubmesh;
int gcurtriangle = -2;
bool gcommonvertex = false;
extern int color;
std::vector<vec>	gtargets;
void resetsm()
{
	gsubmesh.vertices.clear();
	gsubmesh.cors.clear();
	gsubmesh.normals.clear();
	gsubmesh.uvs.clear();
	gsubmesh.tris.clear();
	gcurtriangle = -1;
	gtargets.clear();
}
int getverind(const vertex& v)
{
	submesh& sm = gsubmesh;
	if (gcommonvertex)
	{
		// 寻找同样的顶点
		for (int i = 0; i < sm.vertices.size(); i++)
		{
			if (sm.vertices[i] == v.p)
				return i;
		}
	}
	sm.vertices.push_back(v.p);
	sm.uvs.push_back(v.uv);
	sm.cors.push_back(color);
	return sm.vertices.size() - 1;
}

void addtri2sm(const vertex& p1, const vertex& p2, const vertex& p3)
{
	if (gsubmesh.vertices.size() >= 60000)
	{
		LOGMSG("添加失败 三角形数量超过60000!")
		return;
	}
	if (gcurtriangle == -2)
		return;

	vec norm = (p2 - p1).cross(p3 - p1);
	norm = norm.normcopy();
	if (norm.sqrlen() == 0)
	{
		//PRINT("bad triangle.")
		return; // 无效三角形
	}
	
	triangle tri;
	tri.vertexIndex[0] = getverind(p1);
	tri.vertexIndex[1] = getverind(p2);
	tri.vertexIndex[2] = getverind(p3);

	submesh& sm = gsubmesh;
	sm.tris.push_back(tri);
	gcurtriangle = sm.tris.size() - 1;
}
void exportsm()
{
	//exportdat(gsubmesh);
}
void rendersubmesh()
{
	rendersubmesh(gsubmesh);
}
// ------------------------------------------------
// READ TEMPLATE
std::string readTemplate()
{
	std::stringstream templateString;
	std::ifstream f("output/model.cs");
	if (f)
	{
		std::copy(std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>(),
			std::ostreambuf_iterator<char>(templateString));
		f.close();
	}
	return templateString.str();
}
// ------------------------------------------------
// SAVE MODEL
std::string SSPRINT(char *pszFormat, ...)
{
	static char sbuf[512];
	va_list  pArgList;
	va_start(pArgList, pszFormat);
	vsprintf_s(sbuf, pszFormat, pArgList);
	va_end(pArgList);
	return sbuf;
}
void exportModel(const std::string& name)
{
	const submesh& sm = gsubmesh;
	std::string str;
	{// 3d data

		str += SSPRINT("//----- vertices %d ------\n", sm.vertices.size());
		str += SSPRINT("float[,] vertices = {\n");

		for (int i = 0; i < sm.vertices.size(); i++)
		{
			const vec& v = sm.vertices[i];
			str += SSPRINT("{%ff, %ff, %ff},\n", v.x, v.y, v.z);
		}
		str += SSPRINT("};\n");

		str += SSPRINT("//----- triangles %d------\n", sm.tris.size());
		str += SSPRINT("int[] triangles = new int [] {\n");
		for (int i = 0; i < sm.tris.size(); i++)
		{
			const triangle& tri = sm.tris[i];
			str += SSPRINT("%d, %d, %d,\n", tri.vertexIndex[0], tri.vertexIndex[1], tri.vertexIndex[2]);
		}
		str += SSPRINT("};\n");
	}
	{// export script
		std::string tmpstr = readTemplate();
		{//model	
			std::string targetstring = "model";
			std::size_t pos = tmpstr.find(targetstring);
			if (pos != std::string::npos)
				tmpstr.replace(pos, targetstring.length(), name);
			pos = tmpstr.find(targetstring);
			if (pos != std::string::npos)
				tmpstr.replace(pos, targetstring.length(), name);
		}
		{//data		
			std::string targetstring = "__MODEL_DATA__";
			std::size_t pos = tmpstr.find(targetstring);
			if (pos != std::string::npos)
				tmpstr.replace(pos, targetstring.length(), str);
		}
		std::string filename = "output/";
		filename += name;
		filename += ".cs";
		std::ofstream f(filename.c_str());
		if (f)
		{
			f << tmpstr;
			f.close();
		}
	}
}

// **********************************************************************
// DLL
// **********************************************************************
#if _MSC_VER
#define EXPORT_API _declspec(dllexport)
#else
#define EXPORT_API
#endif
extern void drawScene(const char* path);
extern int editface(int type, VECLIST& e, const vec& startpos, const vec& updir, real hitdis);
extern submesh gsubmesh;
extern void resetsm();
extern void exportsm();
extern vec getedgenorm(const VECLIST& e);
extern void plane(const vertex& p1, const vertex& p2, const vertex& p3, const vertex& p4, int cor);
extern void triang(const vertex& p1, const vertex& p2, const vertex& p3, int cor);

extern "C"
{
	// ---------------------------------------------------------
	// 独立绘制场景
	// ---------------------------------------------------------
	void EXPORT_API drawscene(const char* path)
	{
		MAXZ = 8.0f;
		renderstate = 1;
		drawScene(path);
		MAXZ = 0;
	}
	// ---------------------------------------------------------
	// 编辑面
	// ---------------------------------------------------------
	bool isOnSigment(const vec& p1, const vec& p2, const vec& p)
	{
		if (p == p1 || p == p2)
			return false; // 不允许在顶点上
		return ((p - p1).len() + (p - p2).len() - (p1 - p2).len()) < 0.000001f;
	}
	vec gettrinorm(const vec& p1, const vec& p2, const vec& p3)
	{
		return (p2 - p1).cross((p3 - p1)).normcopy();
	}
	real gettrinormval(const vec& p1, const vec& p2, const vec& p3)
	{
		return (p2 - p1).cross((p3 - p1)).len();
	}
	bool isnormV(const vec& n1, const vec& p1, const vec& p2, const vec& p3)
	{		
		return abs(n1.dot((p2 - p1).cross((p3 - p1)).normcopy())) < EPSINON * 0.1;
	}
	bool isnormVex(const vec& n1, const vec& p1, const vec& p2, const vec& p3, const vec& dir)
	{
		vec tdir = (p2 - p1).cross((p3 - p1)).normcopy();
		return abs(n1.dot(tdir)) < EPSINON * 0.1 && (n1.cross(tdir).dot(dir) > 0);
	}
	bool iserrtriangle(const vec& p1, const vec& p2, const vec& p3)
	{		
		vec::sEPSINON = EPSINON * EXPORT_SCALE;
		bool ret = (p1 == p2 || p1 == p3 || p2 == p3);
		vec::sEPSINON = EPSINON;
		if (ret)
			logmessage << "\n triangle err!";
		return ret;
	}
	real getsin(const vec& p1, const vec& p2, const vec& p3)
	{
		return (p2 - p1).normcopy().cross((p3 - p1).normcopy()).len();
	}
	// ---------------------------------------------------------
	int EXPORT_API getnexttri(vec* v, int vcnt, int* t, int tcnt, int tri1)
	{
		int tri2 = tri1 - 3;
		if(tri2 >= 0)
		{
			vec& p1 = v[tri1];
			vec& p3 = v[tri1 + 1];
			vec& p2 = v[tri1 + 2];

			vec& pp1 = v[tri2];
			vec& pp3 = v[tri2 + 1];
			vec& pp2 = v[tri2 + 2];
			//logmessage << "\n dgettrinormval1 " << (gettrinormval(p1, p2, p3) - gettrinormval(pp1, pp2, pp3));
			if (!iserrtriangle(p1, p2, p3)
				&& (p2 == pp3 && p3 == pp2) // 重合2个顶点
				&& (abs(gettrinormval(p1, p2, p3) - gettrinormval(pp1, pp2, pp3)) < 0.0005) // 两个三角形面积相仿
				)
				return tri2;
		}
		tri2 = tri1 + 3;
		if (tri2 < tcnt)
		{
			vec& p1 = v[tri1];
			vec& p3 = v[tri1 + 1];
			vec& p2 = v[tri1 + 2];

			vec& pp1 = v[tri2];
			vec& pp3 = v[tri2 + 1];
			vec& pp2 = v[tri2 + 2];
			//logmessage << "\n dgettrinormval2 " << (gettrinormval(p1, p2, p3) - gettrinormval(pp1, pp2, pp3));
			if (!iserrtriangle(p1, p2, p3)
				&& (p2 == pp3 && p3 == pp2) // 重合2个顶点
				&& (abs(gettrinormval(p1, p2, p3) - gettrinormval(pp1, pp2, pp3)) < 0.0005) // 两个三角形面积相仿
				)
				return tri2;
		}	
		//logmessage << "\n getnexttri failed";
		return -1;		
	}

	// ---------------------------------------------------------
	bool EXPORT_API colorface(unsigned int* vcor, int vcnt, int* t, int tcnt, int tri1, int tri2, unsigned int cor)
	{
		if (tri1 >= 0 && tri1 < tcnt)
		{
			vcor[t[tri1]] = blendcor(vcor[t[tri1]], cor, 0.1);
			vcor[t[tri1] + 1] = blendcor(vcor[t[tri1] + 1], cor, 0.1);
			vcor[t[tri1] + 2] = blendcor(vcor[t[tri1] + 2], cor, 0.1);
		}
		if (tri2 >= 0 && tri2 < tcnt)
		{
			vcor[t[tri2]] = blendcor(vcor[t[tri2]], cor, 0.1);
			vcor[t[tri2] + 1] = blendcor(vcor[t[tri2] + 1], cor, 0.1);
			vcor[t[tri2] + 2] = blendcor(vcor[t[tri2] + 2], cor, 0.1);
		}
		return true;
	}
	// ---------------------------------------------------------
	bool EXPORT_API setface(vec* v, int* t, int tcnt, int tri1, int tri2, vec* pt)
	{
		VECLIST e;
		{// triangle1
			v[t[tri1]] = pt[0];
			v[t[tri1] + 1] = pt[3];
			v[t[tri1] + 2] = pt[1];
		}
		{// triangle2
			v[t[tri2]] = pt[2];
			v[t[tri2] + 1] = pt[1];
			v[t[tri2] + 2] = pt[3];
		}
		return true;
	}
	// ---------------------------------------------------------
	void deleteTriangle(vec &p1, vec &p2, vec &p3)
	{
		p2 = blend(p1, p2, 0.01);
		p3 = blend(p1, p3, 0.01);
	}
	// ---------------------------------------------------------
	// 在面上进行3D互动建模
	real gcamdis = 0;
	int dragwid = 0, draghgt = 0, draglen = 0;
	int clipA = 0, clipB = 0;
	extern vec g_vec;
	extern vec getedgecenter(const VECLIST& e);

	int EXPORT_API editface(int type, vec* v, int vcnt, int* t, int tcnt, unsigned int* vcor, int tri1, int tri2, vec campos, vec startpos, vec updir, real hitdis)
	{
		gcamdis = (startpos - campos).len() / EXPORT_SCALE;

		bool bpriview = false;
		if (type >= 1000)
		{
			bpriview = true;
			gblog = false; 			
			type -= 1000;
		}
		else
			gblog = true;
		
		// CHECK
		if (v[t[tri1] + 1] != v[t[tri2] + 2])
		{
			LOGMSG("三角形索引非法!")
				savelog(logmessage.str());
			return false;
		}

//if(!bpriview)
//{// debug
//	LOGMSG("--------------debug tris: --------------------")
//	for (int i = 0; i < tcnt; i += 3)
//	{
//		vec p1 = v[t[i]];
//		vec p2 = v[t[i] + 1];
//		vec p3 = v[t[i] + 2];
//
//		if(iserrtriangle(p1, p2, p3))
//			LOGMSG("err tris: " << t[i] << " p1 = " << VEC2STR(p1) << " p2 = " << VEC2STR(p2) << " p3 = " << VEC2STR(p3))
//		else
//			LOGMSG("tris: " << t[i] << " p1 = " << VEC2STR(p1) << " p2 = " << VEC2STR(p2) << " p3 = " << VEC2STR(p3))
//	}
//}		
		int ret = false;
		if (type == 0 || type == 12)
		{// 延伸
			START_MESH
			unsigned int vcors[4];			
			LOGMSG("--------------延伸edge: --------------------")
			VECLIST e;
			{// triangle1
				vec p1 = v[t[tri1]];
				vec p2 = v[t[tri1] + 1];
				vec p3 = v[t[tri1] + 2];
				e.push_back(p1); e.push_back(p3);
				LOGMSG("triangle1 " << tri1)
			}
			{// triangle2
				vec p1 = v[t[tri2]];
				vec p2 = v[t[tri2] + 1];
				vec p3 = v[t[tri2] + 2];
				e.push_back(p1); e.push_back(p3);
				LOGMSG("triangle2 " << tri2)
			}			
			LOGMSG(VEC2STR(e[0].p) << " " << VEC2STR(e[1].p) << " " << VEC2STR(e[2].p) << " " << VEC2STR(e[3].p))
			vec edgenorm = getedgenorm(e);
			real ustep = USIZE * EXPORT_SCALE;
			real dis = ustep;
			real camdis = (startpos - campos).len();
			real dis0 = MAX((e[0] - e[1]).len(), (e[2] - e[1]).len());

			//if (camdis > dis0 * 6)
			//{// 范围外进行远距离操作
			//	dis = MAX(dis0, ustep);
			//	if (dis > MIN(ustep * 20, camdis)) // 限制每次的移动量
			//		dis = MIN(ustep * 20, camdis);
			//	//dis = int(dis / ustep) * ustep;
			//}
			//else if (camdis > dis0 * 2)
			//{// 范围外进行远距离操作
			//	dis = MAX(dis0 * 0.5, ustep);
			//	if (dis > MIN(ustep * 20, camdis)) // 限制每次的移动量
			//		dis = MIN(ustep * 20, camdis);
			//	dis = int(dis / ustep) * ustep;
			//}
			//dis = MIN(hitdis, dis);
			//if (hitdis < ustep)
			//{
			//	draglen = 1;
			//	dis = hitdis; // 直接拖拽
			//}
			//else
			//{
				draglen = int(hitdis / ustep + 0.5);
				dis = hitdis;//int(hitdis / ustep + 0.5) * ustep; // 直接拖拽
			//}
			dragwid = (e[0] - e[1]).len() / ustep + 0.5;
			draghgt = (e[2] - e[1]).len() / ustep + 0.5;			
			g_vec = getedgecenter(e) + edgenorm * dis;

			if (dis == 0)
				return false;
			if (type == 12)
			{
				hitdis = dis;
				goto EDITFACE;
			}			
			vec deta = edgenorm * dis;
			
			ret = 3;
			{// 操作面
				v[t[tri1]]+= deta;
				v[t[tri1] + 1] += deta;
				v[t[tri1] + 2] += deta;
				v[t[tri2]] += deta;
				v[t[tri2] + 1] += deta;
				v[t[tri2] + 2] += deta;
			}
			{// 判断各个边缘是否存在临面,否则就加面
				for (int j = 0; j < 4; j++)
				{	
					bool bsuccess = 0;
					for (int i = 0; i < tcnt; i += 3)
					{
						vec& p1 = v[t[i]];
						vec& p3 = v[t[i] + 1];
						vec& p2 = v[t[i] + 2];

						if (t[i] == tri1 || t[i] == tri2
							|| iserrtriangle(p1, p2, p3) // 无效三角形最好重新利用一下！
							|| getsin(p1, p2, p3) < 0.9 // 必须存在直角
							)
							continue;
						// 含有直角顶点边匹配
						if ((p1 == e[j].p) || p1 == e[(j + 1) % 4].p) // 直角点匹配
						{	
							if (((p2 == e[j].p) || p2 == e[(j + 1) % 4].p || (p3 == e[j].p) || p3 == e[(j + 1) % 4].p) // 另外两个顶点的匹配
								&& isnormVex(edgenorm, p1, p2, p3, e[(j + 1) % 4].p - e[j].p)) // 垂直判定()
							{
								LOGMSG("本三角形 " << t[i])
								if ((p2 == e[j].p) || p2 == e[(j + 1) % 4].p)
								{// 判断临近的三角形P3跟本三角形P2匹配									
									if (i - 3 >= 0 && v[t[i - 3] + 1] == p2 && v[t[i - 3] + 2] == p3)
									{
										if (!iserrtriangle(p1 + deta, p2 + deta, p3))
										{
											p1 += deta;
											p2 += deta;
											v[t[i - 3] + 1] += deta;
											bsuccess = 1;
											LOGMSG("1判断临近的三角形P3跟本三角形P2匹配A " << t[i - 3])
										}
										else
										{
											deleteTriangle(p1, p2, p3);
											deleteTriangle(v[t[i - 3]], v[t[i - 3] + 1], v[t[i - 3] + 2]);
											LOGMSG("1判断临近的三角形P3跟本三角形P2匹配A 三角形重合!" << t[i - 3])
										}
										break;
									}
									else if (i + 3 < tcnt && v[t[i + 3] + 1] == p2 && v[t[i + 3] + 2] == p3)
									{
										if (!iserrtriangle(p1 + deta, p2 + deta, p3))
										{
											p1 += deta;
											p2 += deta;
											v[t[i + 3] + 1] += deta;
											bsuccess = 1;
											LOGMSG("1判断临近的三角形P3跟本三角形P2匹配B" << t[i + 3])
										}
										else
										{
											deleteTriangle(p1, p2, p3);
											deleteTriangle(v[t[i + 3]], v[t[i + 3] + 1], v[t[i + 3] + 2]);
											LOGMSG("1判断临近的三角形P3跟本三角形P2匹配B 三角形重合!" << t[i + 3])
										}
										break;
									}
								}
								else if ((p3 == e[j].p) || p3 == e[(j + 1) % 4].p)
								{// 判断临近的三角形P2跟本三角形P3匹配									
									if (i - 3 >= 0 && v[t[i - 3] + 2] == p3 && v[t[i - 3] + 1] == p2)
									{		
										if (!iserrtriangle(p1 + deta, p2, p3 + deta))
										{
											p1 += deta;
											p3 += deta;
											v[t[i - 3] + 2] += deta;
											bsuccess = 1;
											LOGMSG("2判断临近的三角形P2跟本三角形P3匹配A" << t[i - 3])
										}
										else
										{
											deleteTriangle(p1, p2, p3);
											deleteTriangle(v[t[i - 3]], v[t[i - 3] + 1], v[t[i - 3] + 2]);
											LOGMSG("2判断临近的三角形P2跟本三角形P3匹配A 三角形重合!" << t[i - 3])
										}
										break;
									}
									else if (i + 3 < tcnt && v[t[i + 3] + 2] == p3 && v[t[i + 3] + 1] == p2)
									{
										if (!iserrtriangle(p1 + deta, p2, p3 + deta))
										{
											p1 += deta;
											p3 += deta;
											v[t[i + 3] + 2] += deta;
											bsuccess = 1;
											LOGMSG("2判断临近的三角形P2跟本三角形P3匹配B" << t[i + 3])
										}
										else
										{
											deleteTriangle(p1, p2, p3);
											deleteTriangle(v[t[i + 3]], v[t[i + 3] + 1], v[t[i + 3] + 2]);
											LOGMSG("2判断临近的三角形P2跟本三角形P3匹配B 三角形重合!" << t[i + 3])
										}										
										break;
									}
								}
							}
						}
					}
					if(!bsuccess)
					{//加面						
						plane(e[j].p / EXPORT_SCALE, e[(j + 1) % 4].p / EXPORT_SCALE, e[(j + 1) % 4].p / EXPORT_SCALE + deta / EXPORT_SCALE, e[j].p / EXPORT_SCALE + deta / EXPORT_SCALE, 0xFFFF00FF);
						LOGMSG("加面")
						ret = 1;
					}
				}
			}
			savelog(logmessage.str());
			END_MESH
			return ret;		
		}
	EDITFACE:
		START_MESH
		{// 编辑面
			LOGMSG("--------------编辑面edge: "<< type << "--------------------")
			VECLIST e;
			{// triangle1
				vec p1 = v[t[tri1]];
				vec p2 = v[t[tri1] + 1];
				vec p3 = v[t[tri1] + 2];
				e.push_back(p1 / EXPORT_SCALE); e.push_back(p3 / EXPORT_SCALE);
				LOGMSG("triangle1 " << tri1 << " p1 = " << VEC2STR(p1) << " p2 = " << VEC2STR(p2) << " p3 = " << VEC2STR(p3))
			}
			{// triangle2
				vec p1 = v[t[tri2]];
				vec p2 = v[t[tri2] + 1];
				vec p3 = v[t[tri2] + 2];
				e.push_back(p1 / EXPORT_SCALE); e.push_back(p3 / EXPORT_SCALE);
				LOGMSG("triangle2 " << tri2 << " p1 = " << VEC2STR(p1) << " p2 = " << VEC2STR(p2) << " p3 = " << VEC2STR(p3))
			}			
			LOGMSG(VEC2STR(e[0].p) << " " << VEC2STR(e[1].p) << " " << VEC2STR(e[2].p) << " " << VEC2STR(e[3].p))
			ret = editface(type, e, startpos / EXPORT_SCALE, updir.normcopy(), hitdis / EXPORT_SCALE);
		}
		END_MESH
		savelog(logmessage.str());
		return ret;
	}

	extern void flowertriangle(vec p1, vec p2, vec p3);
	int EXPORT_API editfacetri(int type, vec* v, int vcnt, int* t, int tcnt, unsigned int* vcor, int tri1, vec campos, vec startpos, vec updir, real hitdis)
	{
		bool bpriview = false;
		if (type >= 1000)
		{
			bpriview = true;
			gblog = false;
			type -= 1000;
		}
		else
			gblog = true;

		int ret = 0;
		START_MESH
		{// 编辑面
			LOGMSG("--------------editfacetri edge: " << type << "--------------------")
			// triangle1
			vec p1 = v[t[tri1]] / EXPORT_SCALE;
			vec p2 = v[t[tri1] + 1] / EXPORT_SCALE;
			vec p3 = v[t[tri1] + 2] / EXPORT_SCALE;
			LOGMSG("triangle1 " << tri1 << " p1 = " << VEC2STR(p1) << " p2 = " << VEC2STR(p2) << " p3 = " << VEC2STR(p3))
					
			if (type == 35)
			{// 花毯
				color = 0xFF006080;
				vec deta = gettrinorm(p1, p3, p2) * (USIZE * 0.1);
				triang(p1 + deta, p3 + deta, p2 + deta, color);
				color = 0xFFFFFFFF;
				flowertriangle(p1, p2, p3);
				ret = 6;
			}
		}
		END_MESH
		savelog(logmessage.str());
		return ret;
	}
	// ---------------------------------------------------------
	// 在面上进行2D三角面互动造型（类似于剪纸效果）
	// 三角形造型艺术
	// ---------------------------------------------------------
	void triangex(const vec& n0, const vec& p1, const vec& p2, const vec& p3, int cor)
	{
		vec n = (p2 - p1).cross((p3 - p1)).normcopy();
		if (n.dot(n0) > 0)
			triang(p1, p2, p3, cor);
		else
			triang(p1, p3, p2, cor);
	}
	real dis2Sigment(const vec& p1, const vec& p2, const vec& p)
	{		
		return (p - p1).len() + (p - p2).len() - (p1 - p2).len();
	}	
	int EXPORT_API triangleArt(int type, vec* v, int vcnt, int* t, int tcnt, unsigned int* vcor, int tri1, vec pos)
	{
		LOGMSG("--------------triangleArt--------------------")
		if (tri1 >= vcnt)
			return false;
		
		int ret = false;
		vec p1 = v[t[tri1]] / EXPORT_SCALE;
		vec p3 = v[t[tri1] + 1] / EXPORT_SCALE;
		vec p2 = v[t[tri1] + 2] / EXPORT_SCALE;
		vec norm = gettrinorm(p1, p2, p3);		
		pos /= EXPORT_SCALE;
		pos += norm * (rrnd(0, 0.01) * USIZE);
		//real dot1 = -(pos - p1).normcopy().cross((p3 - p1).normcopy()).dot(norm);
		//real dot2 = -(pos - p2).normcopy().cross((p1 - p2).normcopy()).dot(norm);
		//real dot3 = -(pos - p3).normcopy().cross((p2 - p3).normcopy()).dot(norm);

		g_vec /= EXPORT_SCALE;
		real dis1 = (g_vec - p1).len();
		real dis2 = (g_vec - p2).len();
		real dis3 = (g_vec - p3).len();
		const real cmindis = 0.25 * USIZE;
		if(dis1 < cmindis || dis2 < cmindis || dis3 < cmindis)
		{// 调整顶点
			ret = 2;
			if(dis1 < cmindis)
				v[t[tri1]] = pos * EXPORT_SCALE;
			else if (dis2 < cmindis)
				v[t[tri1] + 2] = pos * EXPORT_SCALE;
			else if (dis3 < cmindis)
				v[t[tri1] + 1] = pos * EXPORT_SCALE;
		}
		else
		{// 增加边缘
			START_MESH
			vec nn = (p3 - p1).cross(norm).normcopy();
			real dot1 = (g_vec - p1).dot(nn);
			nn = (p1 - p2).cross(norm).normcopy();
			real dot2 = (g_vec - p2).dot(nn);
			nn = (p2 - p3).cross(norm).normcopy();
			real dot3 = (g_vec - p3).dot(nn);
			{
				ret = 1;
				if (dot3 < dot2 && dot3 < dot1)
				{
					logmessage << "\ntriangleArt 1";
					triangex(norm, p2, pos, p3, 0xFFFFFFFF);
				}
				else if (dot2 < dot3 && dot2 < dot1)
				{
					logmessage << "\ntriangleArt 2";
					//LOGMSG(VEC2STR(p1) << ", " << VEC2STR(pos) << ", " << VEC2STR(p2))
					triangex(norm, p1, pos, p2, 0xFFFFFFFF);
				}
				else
				{
					logmessage << "\ntriangleArt 3";
					triangex(norm, p1, p3, pos, 0xFFFFFFFF);
				}
			}
			END_MESH
		}		
		return ret;
	}
	// ---------------------------------------------------------
	// 三角形雕塑
	// ---------------------------------------------------------
	extern void face(const VECLIST& edge, vec o);
	int EXPORT_API triangleArt3D(int type, vec* v, int vcnt, int* t, int tcnt, unsigned int* vcor, int tri1, vec pos)
	{
		LOGMSG("--------------triangleArt3d--------------------")
			if (tri1 >= vcnt)
				return false;

		int ret = false;
		vec p1 = v[t[tri1]] / EXPORT_SCALE;
		vec p3 = v[t[tri1] + 1] / EXPORT_SCALE;
		vec p2 = v[t[tri1] + 2] / EXPORT_SCALE;
		vec norm = gettrinorm(p1, p2, p3);
		pos /= EXPORT_SCALE;
		pos += norm * (rrnd(0, 0.01) * USIZE);

		g_vec /= EXPORT_SCALE;
		real dis1 = (g_vec - p1).len();
		real dis2 = (g_vec - p2).len();
		real dis3 = (g_vec - p3).len();
		const real cmindis = 0.25 * USIZE;
		if (dis1 < cmindis || dis2 < cmindis || dis3 < cmindis)
		{// 调整顶点
			ret = 2;
			if (dis1 < cmindis)
				v[t[tri1]] = pos * EXPORT_SCALE;
			else if (dis2 < cmindis)
				v[t[tri1] + 2] = pos * EXPORT_SCALE;
			else if (dis3 < cmindis)
				v[t[tri1] + 1] = pos * EXPORT_SCALE;
		}
		else
		{// 增加边缘
			START_MESH				
			ret = 1;
			logmessage << "\ntriangleArt3d 3";
			VECLIST ee;
			ee.push_back(p1); ee.push_back(p2); ee.push_back(p3); ee.push_back(p1);
			face(ee, pos);
			
			END_MESH
		}
		return ret;
	}

	// ---------------------------------------------------------
	// 方格花边装饰
	// ---------------------------------------------------------
	void planeex(const vec& n0, const vec& p1, const vec& p2, const vec& p3, const vec& p4, int cor)
	{
		triangex(n0, p2, p3, p1, cor);
		triangex(n0, p4, p1, p3, cor);
	}
	int EXPORT_API blockArt(int type, vec* v, int vcnt, int* t, int tcnt, unsigned int* vcor, int tri1, vec pos)
	{
		LOGMSG("--------------blockArt--------------------")
		if (tri1 >= vcnt)
			return false;
		START_MESH
		int ret = false;
		vec p1 = v[t[tri1]] / EXPORT_SCALE;
		vec p3 = v[t[tri1] + 1] / EXPORT_SCALE;
		vec p2 = v[t[tri1] + 2] / EXPORT_SCALE;
		vec norm = gettrinorm(p1, p2, p3);
		pos /= EXPORT_SCALE;
		pos += norm * (rrnd(0, 0.01) * USIZE);

		real dot1 = -(pos - p1).normcopy().cross((p3 - p1).normcopy()).dot(norm);
		real dot2 = -(pos - p2).normcopy().cross((p1 - p2).normcopy()).dot(norm);
		real dot3 = -(pos - p3).normcopy().cross((p2 - p3).normcopy()).dot(norm);		

		logmessage << "\ndot1=" << dot1 << "\ndot2=" << dot2 << "\ndot3=" << dot3;
		ret = 1;
		if (dot3 > dot2 && dot3 > dot1)
		{
			vec deta = (p3 - p2).cross(norm).normcopy();
			deta *= ((pos - p2).dot(deta));

			logmessage << "\nblockArt 2 " << deta.len();
			planeex(norm, p2, p2 + deta, p3 + deta, p3, 0xFFFFFFFF);
		}
		else if (dot1 > dot2 && dot1 > dot3)
		{
			vec deta = (p1 - p3).cross(norm).normcopy();
			deta *= ((pos - p3).dot(deta));

			logmessage << "\nblockArt 3 " << deta.len();
			planeex(norm, p3, p3 + deta, p1 + deta, p1, 0xFFFFFFFF);
		}
		END_MESH
		return ret;
	}
	// ---------------------------------------------------------
	// 反向法线
	// ---------------------------------------------------------
	void EXPORT_API invnorm(int* t, int tcnt)
	{
		for (int i = 0; i < tcnt; i+=3)
		{
			int& p1 = t[i];
			int& p2 = t[i + 1];
			int& p3 = t[i + 2];
			int t = p2;
			p2 = p3;
			p3 = t;
		}
	}
	// ---------------------------------------------------------
	// 设置当前方位
	// ---------------------------------------------------------
	bool gIsSetDxDy = 0;
	vec gDX, gDY;
	void EXPORT_API setdxdy(vec dx, vec dy)
	{
		gIsSetDxDy = 1;
		gDX = dx;
		gDY = dy;
	}
	// ---------------------------------------------------------
	// 导出几何数据
	// ---------------------------------------------------------
	int EXPORT_API getVcnt()
	{
		return gsubmesh.vertices.size();
	}
	bool EXPORT_API getVdata(vec* v)
	{
		for (int i = 0; i < gsubmesh.vertices.size(); i++)
		{
			v[i] = gsubmesh.vertices[i];
		}
		return true;
	}
	bool EXPORT_API getCdata(int* c)
	{
		for (int i = 0; i < gsubmesh.vertices.size(); i++)
		{
			c[i] = gsubmesh.cors[i];
		}
		return true;
	}	
	bool EXPORT_API getUVdata(vec2* v)
	{
		for (int i = 0; i < gsubmesh.uvs.size(); i++)
		{
			v[i] = gsubmesh.uvs[i];
		}
		return true;
	}
	int EXPORT_API getTcnt()
	{
		return gsubmesh.tris.size() * 3;
	}
	bool EXPORT_API getTdata(int* t)
	{
		int cnt = 0;
		for (int i = 0; i < gsubmesh.tris.size(); i++)
		{
			t[cnt++] = gsubmesh.tris[i].vertexIndex[0];
			t[cnt++] = gsubmesh.tris[i].vertexIndex[1];
			t[cnt++] = gsubmesh.tris[i].vertexIndex[2];
		}
		return true;
	}
	// ---------------------------------------------------------
	// 导出挂点信息
	// ---------------------------------------------------------
	int EXPORT_API gettargetcnt()
	{
		return gtargets.size();
	}
	bool EXPORT_API getTargets(vec* p)
	{
		for (int i = 0; i < gtargets.size(); i++)
		{
			p[i] = gtargets[i] * EXPORT_SCALE;
		}
		return true;
	}
	// ---------------------------------------------------------
	// 内存操作
	// ---------------------------------------------------------
	void EXPORT_API converVecs2Bytes(vec* src, unsigned char* des, int sz)
	{
		memcpy_s(des, sz, src, sz);
	}
	void EXPORT_API converInts2Bytes(int* src, unsigned char* des, int sz)
	{
		memcpy_s(des, sz, src, sz);
	}
	void EXPORT_API converCors2Bytes(unsigned int* src, unsigned char* des, int sz)
	{
		memcpy_s(des, sz, src, sz);
	}
	// ---------------------------------------------------------
	// 用户状态控制
	// ---------------------------------------------------------
	int g_int = 0;
	void EXPORT_API setInt(int i)
	{
		g_int = i;
	}
	int EXPORT_API getInt()
	{
		return g_int;
	}
	unsigned int g_uint = 0xFF8000FF;
	void EXPORT_API setUInt(unsigned int i)
	{
		g_uint = i;
	}
	unsigned int EXPORT_API getUInt()
	{
		return g_uint;
	}
	real g_real = 0;
	void EXPORT_API setReal(real f)
	{
		g_real = f;
	}
	real EXPORT_API getReal()
	{
		return g_real;
	}
	real g_real2 = 0;
	void EXPORT_API setReal2(real f)
	{
		g_real2 = f;
	}
	real EXPORT_API getReal2()
	{
		return g_real2;
	}
	vec g_vec;
	void EXPORT_API setVec(vec v)
	{
		g_vec = v;
	}
	vec EXPORT_API getVec()
	{
		return g_vec;
	}
	// 延伸信息
	int EXPORT_API getdraglen()
	{
		return draglen;
	}
	int EXPORT_API getdragwid()
	{
		return dragwid;
	}
	int EXPORT_API getdraghgt()
	{
		return draghgt;
	}
	// 分割信息
	int EXPORT_API getclipA()
	{
		return clipA;
	}
	int EXPORT_API getclipB()
	{
		return clipB;
	}
}