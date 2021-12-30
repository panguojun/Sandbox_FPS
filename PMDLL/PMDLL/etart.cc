#pragma once
#include <map>
#include <string>
#include "com.hpp"

using namespace std;
// ################################################################################################
// 太空漫游
// 小件编程，比例编程
// 太空蒸汽朋克
// 乐高积木 minecraft cubeworld
// 由形入质，细分造型
// 玩法分 漫游 vs 生存
// 意向性 与开放式漫游, 沙盘去掉意向性
// 怪物PM, 孢子
// 边角模式, 夹心饼干
// 什么都不像，世界留给观者定义
// 太空城
// 太空山体+几何建筑
// 轨道模拟
// 机器艺术
// 建筑艺术
// 生成一切动态元素
// UI
// UIPM
// PM怪
// 天南星植物
// PM机械，互动机械
// 农业
// 建造形式系统基本版
// 种子与智能材料
// ################################################################################################
vec CAM_POS	= vec::CENTER;
inline void viewprj(const vector3& rp, vector2& p2d, real& depth)
{
	vec p = rp;
	p -= CAM_POS;
	p.rot(PI * 0.25, vec::UY);
	p.rot(PI * 0.15, vec::UX);

	p += CAM_POS;
	p.z += MAXZ / 3;
	depth = p.z;
	real vpf = blend(4, 0.1, p.z / MAXZ);
	p2d.x = 0.5 + p.x * vpf;
	p2d.y = 0.25 + p.y * vpf;
//PRINT(p2d.x << ", " << p2d.y)	
}
// -------------------------------------------------
real getviewdis(const vec& p1, const vec& p2)
{
	real depth;
	vec2 pp1, pp2;
	viewprj(p1, pp1, depth);
	viewprj(p2, pp2, depth);
	return (pp1 - pp2).len() * (IMAGESCALE * 1.2);
}
// -------------------------------------------------
double RidgedMFex(double x, double y, double z, int octaves, float offset)
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
// -------------------------------------------------
vec gworldpos;
real fixedSZ(real x)
{	
	return int((x) / USIZE + 0.5) * USIZE;
}
real fixedSZfloor(real x)
{
	return int((x) / USIZE) * USIZE;
}

real fract(real v)
{
	return v - int(v);
}
real rndbypos2(const vec2& pos, real min = 0, real max = 1)
{
	float h = vec2(pos.x, pos.y).dot(vec2(127.1, 311.7));
	return blend(min, max, fract(sin(h) * 43758.5453123));
}
real rndbypos(crvec pos, real min = 0, real max = 1)
{
	float h = vec2(pos.x, pos.z).dot(vec2(127.1, 311.7));
	return blend(min, max, fract(sin(h + fract(pos.y)) * 43758.5453123));
}
real rndbypos(real min = 0, real max = 1)
{
	float h = vec2(gworldpos.x, gworldpos.z + gworldpos.y).dot(vec2(127.1, 311.7));
	return blend(min, max, fract(sin(h) * 43758.5453123));
}
// -------------------------------------------------
// 得到向量的两个垂直方向
// -------------------------------------------------
void v2vxvy(const vec& v, vec& vx, vec& vy)
{
	if (gIsSetDxDy)
	{
		vx = gDX; vy = gDY;
		return;
	}
	vec uz = vec::UZ;
	if (v.x < EPSINON && v.x > -EPSINON && v.y < EPSINON && v.y > -EPSINON)
	{
		uz = vec::UX;		
	}
	vy = v.cross(uz); vy.norm();
	vx = vy.cross(v);
}
// -------------------------------------------------
inline vec scaleby(crvec v, crvec o, real s)
{
	return (v - o) * s + o;
}

// ================================================================================================
// 绘制
// 三角形元素
// 渲染光栅化
// ================================================================================================
// 渲染状态
vec light(0.25, 1, -2);		// 全局光的反向
int color = 0xFFFFFFFF;		// 当前色彩
int lummode = 0;			// -1 暗 0 正常 1 亮
bool backfacecull = false;	// 背面剔除
int renderstate = 0;		// 渲染模式 0 不渲染， 1 正常渲染 2 网格渲染
bool binvnorm = false;
// ------------------------------------------------------------------------------------------------
void triang(const vertex& p1, const vertex& p2, const vertex& p3, int cor = 0xFFFFFFFF)
{
	if(binvnorm)
		addtri2sm(p1 * EXPORT_SCALE, p2 * EXPORT_SCALE, p3 * EXPORT_SCALE);
	else
		addtri2sm(p1 * EXPORT_SCALE, p3 * EXPORT_SCALE, p2 * EXPORT_SCALE);

	if(renderstate == 0)
		return;
	else if(renderstate == 1)
	{
		vec norm = (p2 - p1).cross(p3 - p1);
		norm = norm.normcopy();
		if(backfacecull && norm.z > 0)
			return;
		if(norm.sqrlen() == 0)
		{
			//pixelAA((p1 + p2 + p3) / 3.0, cor);
			//line3d(p1, p2, cor);
			//line3d(p1, p3, cor);
			//line3d(p2, p3, cor);
			return; // 无效三角形
		}		
		if(color == 0xFFFFFFFF)
		{
			int cor0 = 0x000000FF & cor;
			if(lummode == 1)
			{
				int lum = blend(cor0, 255, norm.dot(light) * 1.5, 3);			
				cor = RGB(lum, lum, lum);
			}
			else if(lummode == -1)
			{
				int lum = blend(cor0, 188, norm.dot(light), 2);
				cor = RGB(lum, lum, lum);	
			}
			else
			{
				int lum = blend(cor0, 255, norm.dot(light));
				cor = RGB(lum, lum, lum);
			}			
		}
		else
		{
			int lum = blend(0, 255, norm.dot(light) * 1.5, 3);
			cor = RGB(lum, lum, lum);
			cor = blendcor(cor, color, 0.5);			
		}	
		LOOP(i, len, getviewdis(p1.p, p2.p))
			vec p12 = blend(p1.p, p2.p, ai);
			LOOP(j, wid, getviewdis(p12, p3.p))
				vec p = blend(p12, p3.p, aj);
				pixel(p, cor);	
			END
		END

	}
	else if(renderstate == 2)
	{
		line3d(p1.p, p2.p, cor);
		line3d(p2.p, p3.p, cor);
		line3d(p3.p, p1.p, cor);
	}
}
// -------------------------------------------------
// 面
// -------------------------------------------------
void plane(const vertex& p1, const vertex& p2, const vertex& p3, const vertex& p4, int cor = 1)
{
	/*triang(p1, p2, p3, cor);
	triang(p1, p3, p4, cor);*/
	triang(p2, p3, p1, cor);
	triang(p4, p1, p3, cor);
}

// -------------------------------------------------
// 边缘->面
// -------------------------------------------------
int drawmask = 0xFFFFFFFF;
inline int checkflag(int i)
{
	return drawmask & (1 << i);
}
inline int unflag(int i)
{
	return drawmask &= ~(1 << i);
}
void face(const VECLIST& edge, bool invnorm = false)
{	
	bool oldbinvnorm = binvnorm; binvnorm = invnorm;
	int cor = RNDBW;
	int s = edge.size();
	if(s < 3)
		return;
	if (s == 5)
	{
		plane(edge[0].p, edge[1].p, edge[2].p, edge[3].p, cor);
	}
	else
	{
		vec p0 = edge[0].p;
		for (int i = 0; i < s - 1; i++)
		{
			if (i > 0 && checkflag(i))
				triang(p0, edge[i].p, edge[(i + 1) % s].p, cor);
		}
	}
	drawmask = 0xFFFFFFFF;
	binvnorm = oldbinvnorm;
}
void face(const VECLIST& edge, vec o)
{	
	int cor = RNDBW;
	int s = edge.size();
	if(s < 3)
		return;	
	for(int i = 0; i < s - 1; i ++)
	{		
		if(checkflag(i))
			triang(o, edge[i].p, edge[(i + 1) % s].p, cor);
	}
	drawmask = 0xFFFFFFFF;
}
void face(const VECLIST& edge1, const VECLIST& edge2)
{
	if(edge1.size() != edge2.size())
	{
		LOGMSG("draw err! " << edge1.size() << " != " << edge2.size());
		return;
	}
	
	for(int i = 0; i < edge1.size() - 1; i ++)
	{
		if(checkflag(i))
			plane(edge1[i].p, edge1[i + 1].p, edge2[i + 1].p, edge2[i].p, RNDBW);
	}
	drawmask = 0xFFFFFFFF;
}

// -------------------------------------------------
// 边逻辑
// -------------------------------------------------
VECLIST* pworkingedge = 0; 	// 当前工作面
int workingedgeindex = 0;	 		// 当前工作面索引
// -------------------------------------------------
// 法线
vec getedgenorm(const VECLIST& e)
{
	if(e.size() < 3)
		return vec::ZERO;
	vec norm = (e[1].p - e[0].p).cross(e[2].p - e[0].p);norm.norm();
	return norm;
}
// 平均点
vec getedgecenter(const VECLIST& e)
{
	vec o;
	for(int i = 0; i < e.size(); i ++)
		o = o + e[i].p;
	o = o / e.size();
	return o;
}
// -------------------------------------------------
// 缩放边缘
void scaleedge(const VECLIST& e1, real scale, VECLIST& e2)
{
	int len = e1.size();
	if(len <= 1)
		return;
	if(e1[0].p == e1[len - 1].p)
		len --;		
	vec o;
	for(int i = 0; i < len; i ++)
		o = o + e1[i].p;
	o = o / len; // 平均点
	
	for(int i = 0; i < e1.size(); i ++)
	{
		e2.PUSH((e1[i].p - o) * scale + o);
	}
}
// -------------------------------------------------
void scaleedge(VECLIST& e1, real scale)
{
	int len = e1.size();
	if(len <= 1)
		return;
	if(e1[0] == e1[len - 1])
		len --;	
	vec o;
	for(int i = 0; i < len; i ++)
		o = o + e1[i];
	o = o / len; // 平均点
	
	for(int i = 0; i < e1.size(); i ++)
	{
		e1[i] = ((e1[i] - o) * scale + o);
	}
}
// -------------------------------------------------
void scaleedge(VECLIST& e1, real sx, real sy, real sz, VECLIST& e2)
{
	if(e1.size() == 0)
		return;
	vec o;
	for(int i = 0; i < e1.size(); i ++)
		o = o + e1[i].p;
	o = o / e1.size(); // 平均点
	
	for(int i = 0; i < e1.size(); i ++)
	{
		vec v = (e1[i].p - o);
		v.x *= sx;
		v.y *= sy;
		v.z *= sz;
		e2.PUSH(v + o);
	}
}
// -------------------------------------------------
void scaleedge(VECLIST& e1, real sx, real sy, real sz)
{
	if(e1.size() == 0)
		return;
	vec o;
	for(int i = 0; i < e1.size(); i ++)
		o = o + e1[i];
	o = o / e1.size(); // 平均点
	
	for(int i = 0; i < e1.size(); i ++)
	{
		vec v = (e1[i] - o);
		v.x *= sx;
		v.y *= sy;
		v.z *= sz;
		e1[i] = (v + o);
	}
}
// -------------------------------------------------
// 移动边缘
void moveedge(const VECLIST& e1, vec dv, VECLIST& e2)
{
	for(int i = 0; i < e1.size(); i ++)
	{
		e2.PUSH(e1[i].p + dv);
	}
}
// -------------------------------------------------
void moveedge(VECLIST& e1, vec dv)
{
	for(int i = 0; i < e1.size(); i ++)
	{
		e1[i] = (e1[i] + dv);
	}
}
// -------------------------------------------------
// extrude 边缘
void extrudeedge(const VECLIST& e1, real d, VECLIST& e2)
{
	if(e1.size() < 3)
		return;
	vec norm = (e1[1].p - e1[0].p).cross(e1[2].p - e1[0].p);norm.norm();
	vec dv = norm * d;
	for(int i = 0; i < e1.size(); i ++)
	{
		e2.PUSH(e1[i].p + dv);
	}
}
// -------------------------------------------------
void extrudeedge(VECLIST& e1, real d)
{
	if(e1.size() < 3)
		return;
	vec norm = (e1[1] - e1[0]).cross(e1[2] - e1[0]);norm.norm();
	vec dv = norm * d;
	for(int i = 0; i < e1.size(); i ++)
	{
		e1[i] = (e1[i] + dv);
	}
}

// -------------------------------------------------
// cut 边缘
void cutedge(const VECLIST& ei, int start, int len,  VECLIST& eo)
{
	if(start >= ei.size() || start + len >= ei.size())
		return;
	for(int i = start; i < start + len; i ++)
	{
		eo.PUSH(ei[i]);
	}
}
void cutedge(VECLIST& e, int start, int len)
{
	if(start >= e.size() || start + len >= e.size())
		return;
	int ii = 0;
	for(int i = start; i < start + len; i ++)
	{
		e[ii ++] = e[i];
	}
}
void cutedge(const VECLIST* ei, int pos,  VECLIST* e1 = 0, VECLIST* e2 = 0)
{
	if(!ei || pos >= ei->size())
		return;
	if(e1)
	{
		for(int i = 0; i < pos; i ++)
		{
			e1->PUSH((*ei)[i]);
		}
	}
	if(e2)
	{
		for(int i = pos; i < ei->size(); i ++)
		{
			e2->PUSH((*ei)[i]);
		}
	}
}
// -------------------------------------------------
// subedge 边缘
void subedge(const VECLIST& e, int start, int sublen, VECLIST& se)
{
	int len = e.size();
	for(int i = start; i < start + sublen; i ++)
	{
		int ii = i % len;
		se.PUSH(e[ii]);
	}
}
// -------------------------------------------------
// union 边缘
void unionedge(const VECLIST& e1, const VECLIST& e2, int start, int len, VECLIST& eo) // 注意要逆时针方向, e2前, e1后关系
{
	if(len > e1.size() || len > e2.size())
		return;
	
	int sz1 = e1.size();
	for(int i = start; i < start + len; i ++)
	{
		eo.PUSH(e1[i % sz1]);
	}
	int sz2 = e2.size();
	for(int i = start + len - 1; i >= start; i --)
	{
		eo.PUSH(e2[i % sz2]);
	}	
}
// -------------------------------------------------
// 反向
void invedge(VECLIST& e)
{
	VECLIST ne;
	for(int i = e.size() - 1; i >= 0; i --)
	{
		ne.PUSH(e[i]);		
	}
	e = ne;
}
void invplaneedge12(VECLIST& e)
{
	if(e.size() < 4) 
		return;
	vec tmp = e[0];
	e[0] = e[1];
	e[1] = tmp;
}
void invplaneedge34(VECLIST& e)
{
	if(e.size() < 4) 
		return;
	vec tmp = e[2];
	e[2] = e[3];
	e[3] = tmp;
}
void invplaneedge13(VECLIST& e)
{
	if(e.size() < 4) 
		return;
	vec tmp = e[0];
	e[0] = e[2];
	e[2] = tmp;
}
void invplaneedge24(VECLIST& e)
{
	if(e.size() < 4) 
		return;
	vec tmp = e[1];
	e[1] = e[3];
	e[3] = tmp;
}
void invplaneedge23(VECLIST& e)
{
	if(e.size() < 4) 
		return;
	vec tmp = e[1];
	e[1] = e[2];
	e[2] = tmp;
}
void invplaneedge14(VECLIST& e)
{
	if(e.size() < 4) 
		return;
	vec tmp = e[0];
	e[0] = e[3];
	e[3] = tmp;
}
// -------------------------------------------------
// 旋转 边缘
void rotedge(VECLIST& e, real ang, const vec& ax)
{
	vec o = getedgecenter(e);
	for(int i = 0; i < e.size(); i ++)
	{
		e[i] = (e[i] - o).rotcopy(ang, ax) + o;
	}
}
void rotedge(const VECLIST& e, real ang, const vec& ax, VECLIST& eo)
{
	vec o = getedgecenter(e);
	for(int i = 0; i < e.size(); i ++)
	{
		eo.PUSH((e[i].p - o).rotcopy(ang, ax) + o);
	}
}
void rotedgebycenter(VECLIST& e, real ang, const vec& ax, const vec& ro)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i] = (e[i] - ro).rotcopy(ang, ax) + ro;
	}
}
void rotedgebycenter(VECLIST& e, real ang, const vec& ax, const vec& ro, VECLIST& oe)
{
	for (int i = 0; i < e.size(); i++)
	{
		oe.PUSH((e[i].p - ro).rotcopy(ang, ax) + ro);
	}
}
void rotedgebynorm(VECLIST& e, const vec& norm)
{	
	vec n = getedgenorm(e);
	if (n == norm || n == -norm)
		return;

	vec ax = n.cross(norm); ax.norm();
	real ang = acos(norm.dot(n));
	
	rotedge(e, ang, ax);	
}
void rotedgebynorm(const VECLIST& e, const vec& norm, VECLIST& eo)
{
	vec n = getedgenorm(e);
	if (n == norm || n == -norm)
		return;
	vec ax = n.cross(norm); ax.norm();
	real ang = acos(norm.dot(n));
	
	rotedge(e, ang, ax, eo);	
}
void rotedgebynorm(VECLIST& e, const vec& norm, const vec& ax)
{
	vec n = getedgenorm(e);
	if (n == norm || n == -norm)
		return;

	real ang = acos(norm.dot(n));

	rotedge(e, ang, ax);
}
void rotedgebynorm(const VECLIST& e, const vec& norm, const vec& ax, VECLIST& eo)
{
	vec n = getedgenorm(e);
	if (n == norm || n == -norm)
		return;
	real ang = acos(norm.dot(n));

	rotedge(e, ang, ax, eo);
}
// -------------------------------------------------
// 双倍点数
void doublevnum(VECLIST& e, int num = 0)
{
	VECLIST te;
	int n = num == 0 ? e.size() - 1 : num;
	
	for(int i = 0; i < n; i ++)
	{		
		te.PUSH(e[i]);
		te.PUSH(e[i]);
	}
	e = te;
}
// 闭合
void closeedge(VECLIST& e)
{
	if(e.empty())
		return;
	
	e.PUSH(e[0]);
}
// 链接
void linkedge(VECLIST& e, VECLIST& le, bool inv = false)
{
	if(inv == false)
	{
		for(int i = 0; i < le.size(); i ++)
		{
			e.PUSH(le[i]);
		}
	}
	else
	{
		for(int i = le.size() - 1; i >= 0; i --)
		{
			e.PUSH(le[i]);
		}
	}
}
// 添加中点
void insertp(VECLIST& e, crvec p, int pos)
{
	VECLIST te;
	for(int i = 0; i < e.size(); i ++)
	{		
		te.PUSH(e[i]);
		if(i == pos)	
			te.PUSH(p);			
	}
	e = te;
}
int insertp(VECLIST& e, crvec p, crvec pre, crvec aft)
{
	int pos = -1;
	VECLIST te;
	int i = 0;
	for(; i < e.size() - 1; i ++)
	{		
		te.PUSH(e[i]);
		if(e[i] == pre && e[i + 1] == aft)
		{		
			pos = i;
			te.PUSH(p);		
		}			
	}
	te.PUSH(e.back());
	e = te;
	return i;
}
void blendedge(const VECLIST& e1, const VECLIST& e2, real alpha, VECLIST& eo)
{
	if(e1.size() != e2.size())
		return;
	for(int i = 0; i < e1.size(); i ++)
	{
		eo.PUSH(blend(e1[i].p, e2[i].p, alpha));
	}	
}
// -------------------------------------------------
// 圆边沿
void roundedge(VECLIST& e, crvec o, crvec vx, crvec vy, real r, int sig, real ang0 = 0)
{
	CIRCLE2(i, r, sig, ang0)
		e.PUSH(o + (vx * dpi.x + vy * dpi.y));		
	END
}
void roundedge(VECLIST& e, crvec o, vec v, real r, int sig, real ang0 = 0)
{
	v.norm();
	vec dx, dy;
	v2vxvy(v, dx, dy);
	roundedge(e, o, dx, dy, r, sig, ang0);
}
// -------------------------------------------------
// match point
int matchpoint(const VECLIST& e, crvec p, real mindis = 0.000001)
{
	for(int i = 0; i < e.size(); i ++)
	{		
		if((p - e[i].p).sqrlen() < mindis)
		{			
			return i;
		}			
	}
	return -1;
}
// -------------------------------------------------
bool matchedge(const VECLIST& re, VECLIST& e)
{
	real dis = (getedgecenter(re) - getedgecenter(e)).len();
	vec v = getedgenorm(re) * dis;
	VECLIST ne;
	for(int i = 0; i < 4; i ++)
	{
		int ind = matchpoint(e, re[i].p + v, 0.00001);
		if(-1 == ind)
			return false;
		ne.PUSH(e[ind]);		
	}
	e = ne;
	return true;
}
// -------------------------------------------------
// 一些常用扩展
// -------------------------------------------------
void extrudeedgeex(VECLIST& e, real d, real s)
{
	VECLIST e1;		
	extrudeedge(e, d, e1);
	scaleedge(e1, s);
	face(e, e1);
//	face(e1);
	e = e1;
}
// -------------------------------------------------
void extrudeedgeex(const VECLIST& e, real d, real s, VECLIST& oe)
{	
	extrudeedge(e, d, oe);
	scaleedge(oe, s);	
}
void scaleedgeex(VECLIST& e, real s)
{
	VECLIST e1;
	scaleedge(e, s, e1);
	face(e, e1);
	e = e1;
}
// ================================================================================================
// 小件编程
// ================================================================================================
// 线
// -------------------------------------------------
void lineex(vec p1, vec p2, real s)
{
	 VECLIST e1, e2;
	 roundedge(e1, p1, p2 - p1, s, 3);
	 roundedge(e2, p2, p2 - p1, s, 3);
	 face(e1, e2);
	
//	line3d(p1, p2, RNDBW);
}
void line(vec p1, vec p2, real s)
{
	 VECLIST e1, e2;
	 roundedge(e1, p1, p2 - p1, s, 3);	
	 face(e1, p2);
}

// -------------------------------------------------
void ballface(crvec o, real r, vec p1, vec p2, vec p3, int depth = 0)
{
	static const int scors[] = {RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR, RNDCOR};
	if(depth == 4)
	{
		triang(p1, p2, p3, scors[depth]);
		return;
	}
	
	vec p12 = blend(p1, p2, 0.5); p12 = o + (p12 - o).normcopy() * r;
	vec p23 = blend(p2, p3, 0.5); p23 = o + (p23 - o).normcopy() * r;
	vec p31 = blend(p3, p1, 0.5); p31 = o + (p31 - o).normcopy() * r;
	
	ballface(o, r, p1, p12, p31, depth + 1);
	ballface(o, r, p2, p23, p12, depth + 1);
	ballface(o, r, p3, p31, p23, depth + 1);
	ballface(o, r, p12,p23, p31, depth + 1);
}

// -------------------------------------------------
void ball(crvec o, real a)
{
	vec p[4];
	real r =  sqrt(6.0) / 4 * a;
	real h =  sqrt(6.0) / 3 * a;
	real rr = sqrt(a * a - h * h);
	vec oo = o - vec::UY * (h / 4);
	for(int i = 0; i < 3; i ++)
	{
		real ang = i / 3.0 * 2 * PI;
		p[i] = oo + vec(rr * cos(ang), 0, rr * sin(ang));		
	}
	p[3] = o + vec::UY * (h * 3 / 4);
		
	ballface(o, r, p[3], p[0], p[1]);
	ballface(o, r, p[3], p[1], p[2]);
	ballface(o, r, p[3], p[2], p[0]);
	ballface(o, r, p[0], p[2], p[1]);
}
// -------------------------------------------------
// 管道细节
// -------------------------------------------------
void pip(VECLIST& e, vec p1, vec p2, real r)
{	
	vec v12 = p2 - p1;
	if(e.empty())
		roundedge(e, p1, v12, r, 8);
	extrudeedgeex(e, v12.len(), 1);	
}
void pip(VECLIST& e, vec o, vec v, real l, real r)
{	
	if(e.empty())
		roundedge(e, o, v, r, 8);
	extrudeedgeex(e, l, 1);	
}
void addpip(VECLIST& e, real l)
{	
	extrudeedgeex(e, l, 1);	
}
void addpipS(VECLIST& e, real s)
{	
	VECLIST ee;
	scaleedge(e, s, ee);	
	face(e, ee);
	e = ee;
}
void addpipL(VECLIST& e, real l, crvec ax)
{		
	// 转折
	VECLIST ee;
	rotedge(e, PI / 2, ax, ee);	
	face(e, ee);	
	
	e = ee;	
}
void addpipR(VECLIST& e, real l, crvec ax)
{	
	// 转折
	VECLIST ee;
	rotedge(e, -PI / 2, ax, ee);	
	face(e, ee);	
	
	e = ee;	
}
// -------------------------------------------------
// radar
void radar(vec o, vec v, real s)
{
	int lstlum = lummode;
	lummode = 1;
	v.norm();
	vec vx, vy;
	v2vxvy(v, vx, vy);
	VECLIST e1;roundedge(e1, o, vx, vy, s / 8, 8);
	VECLIST e2;extrudeedge(e1, s / 2, e2);scaleedge(e2, 8);	
	
	face(e1, e2);	
	
	// 支架
	line(o, o + v * (s * 1.0), s * 0.025);	
	vec to = getedgecenter(e2);
	VECLIST e3; roundedge(e3, blend(o, to, 0.75), vx, vy, s * 0.7, 3);
	for(int i = 0; i < e3.size(); i ++)
	{
		line(o + v * (s * 1.0), e3[i], s * 0.01);		
	}
	lummode = lstlum;
}
// ------------------------------------------------
// 方片结构
// ------------------------------------------------
void plane(VECLIST& be, crvec o, vec up, real sx, real sy)
{
	up.norm();
	vec vx, vy;
	v2vxvy(up, vx, vy);
	
	vec p1 = o - vx * (sx / 2) + vy * (sy / 2);
	vec p2 = o - vx * (sx / 2) - vy * (sy / 2);
	vec p3 = o + vx * (sx / 2) - vy * (sy / 2);
	vec p4 = o + vx * (sx / 2) + vy * (sy / 2);
	be.PUSH(p1);be.PUSH(p2);be.PUSH(p3);be.PUSH(p4);
}
void plane(VECLIST& be, crvec o, vec vx, vec vy, real sx, real sy)
{
	vec p1 = o - vx * (sx / 2) + vy * (sy / 2);
	vec p2 = o - vx * (sx / 2) - vy * (sy / 2);
	vec p3 = o + vx * (sx / 2) - vy * (sy / 2);
	vec p4 = o + vx * (sx / 2) + vy * (sy / 2);
	be.PUSH(p1); be.PUSH(p2); be.PUSH(p3); be.PUSH(p4);
}
// ------------------------------------------------
// 圆形结构
// ------------------------------------------------
void round(VECLIST& be, crvec o, vec up, real s, int num = 8, real ang0 = 0)
{
	up.norm();
	vec vx, vy;
	v2vxvy(up, vx, vy);	
	roundedge(be, o, vx, vy, s, num, ang0);	
}
void round(VECLIST& be, real r)
{
	//renderstate = 2;
	vec o = getedgecenter(be);
	VECLIST e;
	vec dx = be[0] - o; dx.norm();
	vec dy = be[1] - o; dy.norm();
	roundedge(e, o, dx, dy, r / 2, 8);
	
	doublevnum(be, 4);
	closeedge(be);
	face(be, e);

	extrudeedgeex(e, r * 0.25, E);
	extrudeedgeex(e, -r * 0.2, E);
	face(e);
}
void round(VECLIST& be, vec pos, real r)
{	
	vec o = pos;
	VECLIST e;
	vec n = getedgenorm(be);
	vec dx, dy;
	v2vxvy(n, dx, dy);

	roundedge(e, o, dx, dy, r / 2, 8);

	extrudeedgeex(e, r * 0.25, E);
	extrudeedgeex(e, -r * 0.2, E);
	face(e);
}

// -------------------------------------------------
// 建筑物
// -------------------------------------------------
void block(vec o, vec uy, real s)
{
	// 比例
	real s1 = s  * E * E;
	real s2 = s1 * E * E;
	real s3 = s2 * E * E;
	real s4 = s3 * E * E;	
	real s5 = s4 * E * E;
	
	uy.norm();
	vec vx, vy;v2vxvy(uy, vx, vy);
	VECLIST e; roundedge(e, o, vx, vy, s3, 4);closeedge(e);
	
	extrudeedgeex(e, s2, 1);		
}
// ------------------------------------------------
// 金字塔
void jinzita(vec o, vec uy, real s)
{
	// 比例
	DEF_SCALES(s)
	
	uy.norm();
	vec vx, vy;v2vxvy(uy, vx, vy);
	VECLIST e; roundedge(e, o, vx, vy, s, 4);closeedge(e);
	
	face(e, o + uy * (s / 2));	
	
}
// ------------------------------------------------
// 石柱
vec zhuzi(vec o, vec uy, real s)
{
	// 比例
	DEF_SCALES(s)
	
	uy.norm();
	vec vx, vy;v2vxvy(uy, vx, vy);
	VECLIST e; roundedge(e, o, vx, vy, s5, 8);closeedge(e);	
	extrudeedgeex(e, s2, 1);
	return getedgecenter(e);
}
void zhujie(vec o1, vec o2, real r1, real r2, bool bclose = 0)
{	
	VECLIST e1, e2;
	CIRCLE(i, 1, 8)
		e1.push_back(o1 + vec(dpi.x, 0, dpi.y) * r1);
		e2.push_back(o2 + vec(dpi.x, 0, dpi.y) * r2);
	END	
	face(e1, e2);
	if (bclose)
	{
		face(e2);
	}
}
// -------------------------------------------------
vec zhu(crvec o1, crvec o2, real r0)
{
	real r = r0;
	vec lp;
	LOOP(i, len, 4)
		real szf = blend(1, 0.97, ai);
		vec p = blend(o1, o2, ai * szf);		
		if(i > 0)
		{
			zhujie(lp, p, r, r * szf);
			r = r * szf;
		}
		lp = p;
	END	
	vec p = blend(o1, o2, 0.985);
	zhujie(lp, p, r, r0);
	lp = p;		
	p = o2;
	zhujie(lp, p, r0, r0, 1);
	lp = p;	
	return p;
}
vec fangzhu(vec o1, vec o2, vec dx, vec dz, real ro)
{
	//o1 += dx * ro; o1 += dz * ro;
	//o2 += dx * ro; o2 += dz * ro;
	VECLIST e;
	e.push_back(o1 - dx * ro - dz * ro);
	e.push_back(o1 + dx * ro - dz * ro);
	e.push_back(o1 + dx * ro + dz * ro);
	e.push_back(o1 - dx * ro + dz * ro);
	closeedge(e);
	real dis = (o1 - o2).len();
	extrudeedgeex(e, dis, 1);
	face(e);
	return getedgecenter(e);
}

// -------------------------------------------------
// 台阶
// -------------------------------------------------
void staris(vec l1, vec r1, vec l2, vec r2, int num)
{
	//real d = (l1 - l2).len() / 100;
	real d = USIZE;
	vec _l12, _r12;
	LOOP(i, len, num)
		vec l12 = blend(l1, l2, ai);
		vec r12 = blend(r1, r2, ai);
		if(i > 0)
		{
			vec lm(_l12.x, l12.y, _l12.z);
			vec rm(_r12.x, r12.y, _r12.z);
			{
				VECLIST e; e.push_back(lm); e.push_back(l12); e.push_back(r12); e.push_back(rm); e.push_back(lm);
				extrudeedgeex(e, d, 1);
				face(e);
			}
			//plane(lm, l12, r12, rm, RNDBW);			
			//plane(_l12, lm, rm, _r12, RNDBW);	
		}		
		_l12 = l12;
		_r12 = r12;
	END
}

// ------------------------------------------------
// 拱门
void gongmentop(vec p1, vec p2, vec uy, real s)
{
	DEF_SCALES(s);
	vec o = (p1 + p2) / 2;
	vec ov = p1 - o;
	real r0 = fixedSZ(ov.len());
	ov.norm();
	uy.norm();
	vec n = uy.cross(ov);n.norm();
	
	VECLIST e1;
	{
		real r = r0 * 0.8;	
		LOOP(i, len, 7)
			real ang = PI + PI * ai;
			vec p = o + ov.rotcopy(ang, n) * r;
			e1.PUSH(p);
		END
	}
	VECLIST e2;
	{
		real r = fixedSZ(r0 + s / 2);
		e2.PUSH(o - ov * r);
		e2.PUSH(o - (ov - uy) * (r));
		e2.PUSH(o + (ov + uy) * (r));
		e2.PUSH(o + ov * (r));
		doublevnum(e2, 4);
	}

	real deta = fixedSZ(s * (1 + E));
	moveedge(e1, n * deta);
	moveedge(e2, n * deta);
	face(e2, e1);
	VECLIST e3, e4;
	moveedge(e2, -n * (2 * deta), e4);
	face(e4, e2);
	moveedge(e1, -n * (2 * deta), e3);
	face(e1, e3);
	face(e3, e4);

	{
		VECLIST ee;
		ee.push_back(e1[0]);
		ee.push_back(e3[0]);
		ee.push_back(e4[0]);
		ee.push_back(e2[0]);
		closeedge(ee);
		face(ee);
	}
	{
		VECLIST ee;
		ee.push_back(e1[7]);
		ee.push_back(e2[7]);
		ee.push_back(e4[7]);
		ee.push_back(e3[7]);
		closeedge(ee);
		face(ee);
	}
}
// ------------------------------------------------
int fanggongmentopside = 1;
void fanggongmentop(vec p1, vec p2, vec uy, bool hasZhu = true)
{	
	real r0 = (p1 - p2).len() / 2;

	real h = fixedSZ((r0 * 2) / E);
	real dh = h - r0;
	real s = MAX(USIZE / 2, fixedSZ(h / 24));

	if (hasZhu)
	{
		p1 += uy * dh;
		p2 += uy * dh;
	}
	vec o = (p1 + p2) / 2;
	vec ov = (p1 - o) * fanggongmentopside;
	
	ov.norm();
	uy.norm();
	vec n = uy.cross(ov); n.norm();
	VECLIST e1;
	{
		real r = r0 * 0.8;		
		LOOP(i, len, 7)
			real ang = PI + PI * ai;
		vec p = o + ov.rotcopy(ang, n) * r;
		e1.PUSH(p);
		END
	}
	VECLIST e2;
	{
		real r = r0;
		e2.PUSH(o - ov * r);
		e2.PUSH(o - (ov - uy) * (r));
		e2.PUSH(o + (ov + uy) * (r));
		e2.PUSH(o + ov * (r));
		doublevnum(e2, 4);
	}
	real deta = s * 1.01f;
	face(e2, e1);
	VECLIST e3, e4;
	moveedge(e2, -n * (deta), e4);
	face(e4, e2);
	moveedge(e1, -n * (deta), e3);
	face(e1, e3);
	face(e3, e4);
	if(hasZhu)
	{
		VECLIST ee;
		ee.push_back(e1[0]);
		ee.push_back(e3[0]);
		ee.push_back(e4[0]);
		ee.push_back(e2[0]);
		closeedge(ee);
		extrudeedgeex(ee, dh, 1);
		face(ee);
	}
	if (hasZhu)
	{
		VECLIST ee;
		ee.push_back(e1[7]);
		ee.push_back(e2[7]);
		ee.push_back(e4[7]);
		ee.push_back(e3[7]);
		closeedge(ee);
		extrudeedgeex(ee, dh, 1);
		face(ee);
	}
}
// ------------------------------------------------
// 从旁边创建
void fanggongmentop_frmside(vec p1, vec p2, vec n, vec uy, int side)
{
	real dh = (p1 - p2).len();
	real w = dh / (1 / E - 0.5);
	vec o = (p1.y < p2.y) ? p1 : p2;
	fanggongmentopside = side;
	fanggongmentop(o, o + n * (w), uy);
	fanggongmentopside = 1;
}
// ------------------------------------------------
void gongmen(vec p1, vec p2, vec up, real s)
{
	DEF_SCALES(s);
	vec pp1 = zhu(p1, p1 + up * s, fixedSZ(s / 16));
	vec pp2 = zhu(p2, p2 + up * s, fixedSZ(s / 16));
	fanggongmentop(pp1, pp2, up, false);
}
// ------------------------------------------------
void gongmentop_frmside(vec p1, vec p2, vec n, vec uy)
{
	real h = (p1 - p2).len();
	real r0 = h / 2;
	vec o = (p1.y < p2.y) ? p1 : p2;
	real s = MAX(USIZE / 2, fixedSZ((h) / 24));
	gongmen(o, o + n * r0, uy, s);
}

// ------------------------------------------------
// 道路
void road(crvec pp1, crvec pp2, crvec pp3, crvec pp4, real w)
{	
	if(rrnd() < 0.5)
	{
		gongmen((pp1 + pp4) / 2, (pp2 + pp3) / 2, vec::UY, w * 0.25);
	}
	{
		VECLIST e;e.PUSH(pp1);e.PUSH(pp2);e.PUSH(pp3);e.PUSH(pp4);closeedge(e);
		face(e);
	}
	return;
	vec v12 = (pp2 - pp1).normcopy();
	vec v43 = (pp3 - pp4).normcopy();
	{// border1		
		vec tp1 = pp1 - v12 * (w * E2);		
		vec tp4 = pp4 - v43 * (w * E2);
		VECLIST e;e.PUSH(pp1);e.PUSH(pp4);e.PUSH(tp4);e.PUSH(tp1);closeedge(e);
		extrudeedgeex(e, w * E3, 1);
		face(e);		
	}
	{// border2			
		vec tp2 = pp2 + v12 * (w * E2);	
		vec tp3 = pp3 + v43 * (w * E2);
		VECLIST e;e.PUSH(pp3);e.PUSH(pp2);e.PUSH(tp2);e.PUSH(tp3);closeedge(e);
		extrudeedgeex(e, w * E3, 1);
		face(e);		
	}	
}
// ------------------------------------------------
void road(crvec p1, crvec p2, real wid)
{
	vec v12 = (p2 - p1).normcopy();
	vec _v12 = vec::UY.cross(v12);
	vec pp1 = p1 + _v12 * wid;
	vec pp2 = p1 - _v12 * wid;
	vec pp4 = p2 + _v12 * wid;
	vec pp3 = p2 - _v12 * wid;	
	
	road(pp1, pp2, pp3, pp4, wid);
}
// ------------------------------------------------
void road(crvec p1, crvec p2, crvec p3, real w)
{
	if((p2 - p1).cross(p3 - p2).y > 0.00001)
	{		
		vec v12 = (p2 - p1);
		real l12 = v12.len();
		v12.norm();
		
		vec _v12 = vec::UY.cross(v12);		
		vec pp1 = p1 + _v12 * w;
		vec pp2 = p1 - _v12 * w;
		vec pp4 = pp1 + v12 * (l12 - w);
		vec pp3 = pp2 + v12 * (l12 + w);		
		road(pp1, pp2, pp3, pp4, w);
		
		vec v23 = (p3 - p2);
		real l23 = v23.len();
		v23.norm();
	
		road(pp4, pp3, pp3 + v23 * (l23 + w), pp4 + v23 * (l23 - w), w);
	}	
	else if((p2 - p1).cross(p3 - p2).y < -0.00001)
	{		
		vec v12 = (p2 - p1);
		real l12 = v12.len();
		v12.norm();
		
		vec _v12 = vec::UY.cross(v12);		
		vec pp1 = p1 + _v12 * w;
		vec pp2 = p1 - _v12 * w;
		vec pp4 = pp1 + v12 * (l12 + w);
		vec pp3 = pp2 + v12 * (l12 - w);		
		road(pp1, pp2, pp3, pp4, w);
		
		vec v23 = (p3 - p2);
		real l23 = v23.len();
		v23.norm();
	
		road(pp4, pp3, pp3 + v23 * (l23 - w), pp4 + v23 * (l23 + w), w);
	}
	else
	{
		road(p1, p3, w);
	}
}

// ================================================
// 建筑物（互动环境的主体）
// ================================================
real ghitdis = 0;
// 随机窗口
void window(crvec p1, crvec p2, crvec p3, crvec p4, int depth = 0)
{
	DEF_SCALES(1);
	if(depth == 2)
	{		
		VECLIST e;e.PUSH(p1);e.PUSH(p2);e.PUSH(p3);e.PUSH(p4);closeedge(e);			
		extrudeedgeex(e, s6 * E * E * E, 1 - E * E * E);
		color = rrnd(0, 100) < 10 ? 2 : blendcor(0xFF00ffff, 0xFF0000FF, rrnd());
		face(e);		
		color = 0xFFFFFFFF;
		return;
	}
	
	real w = (p1 - p2).len();
	real h = (p3 - p2).len();
	int nw, nh;
	if(w > h)
	{
		int power = int(w / h + 0.5);
		nh = 2;
		nw = nh * power;	
	}
	else
	{
		int power = int(h / w + 0.5);
		nw = 2;
		nh = nw * power;			
	}
	LOOP(i, len, nh)
		vec _p14 = blend(p1, p4, real(i - 1) / len);
		vec p14 = blend(p1, p4, ai);
		vec _p23 = blend(p2, p3, real(i - 1) / len);
		vec p23 = blend(p2, p3, ai);
		if(i > 0)
		{
			LOOP(j, wid, nw)
				if(j > 0)
				{
					vec pp1 = blend(_p14, _p23, real(j - 1) / wid);
					vec pp2 = blend(_p14, _p23, aj);
					vec pp4 = blend(p14, p23, real(j - 1) / wid);
					vec pp3 = blend(p14, p23, aj);
					if(rrnd(0, 100) < 40)
						window(pp1, pp2, pp3, pp4, depth + 1);	
					else
					{
						VECLIST e;e.PUSH(pp1);e.PUSH(pp2);e.PUSH(pp3);e.PUSH(pp4);closeedge(e);						
						face(e);						
					}
				}				
			END
		}
	END
}
// ------------------------------------------------
// 日形窗
void riwindow(crvec p1, crvec p2, crvec p3, crvec p4, int nw = 3, int nh = 4)
{
	DEF_SCALES(1);
	real w = (p1 - p2).len();
	real h = (p3 - p2).len();
	//int nw = 3;
	//int nh = 4;
	LOOP(i, len, nh)
		vec _p14 = blend(p1, p4, real(i - 1) / len);
		vec p14 = blend(p1, p4, ai);
		vec _p23 = blend(p2, p3, real(i - 1) / len);
		vec p23 = blend(p2, p3, ai);
		if (i > 0)
		{
			LOOP(j, wid, nw)
				if (j > 0)
				{
					vec pp1 = blend(_p14, _p23, real(j - 1) / wid);
					vec pp2 = blend(_p14, _p23, aj);
					vec pp4 = blend(p14, p23, real(j - 1) / wid);
					vec pp3 = blend(p14, p23, aj);
					if(j == 2 && (i == 2 || i == 3))
					{					
						VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
						color = 1;
						extrudeedgeex(e, s6 * E * E * E, 1 - E * E * E);
						color = rrnd(0, 100) < 5 ? 2 : blendcor(0xFF40ffff, 0xFF4040FF, rrnd());
						face(e);
						color = 0xFFFFFFFF;					
					}
					else
					{
						VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
						face(e);
					}
				}
			END
		}
	END
}
// ------------------------------------------------
// 方形窗
void fangwindow(crvec p1, crvec p2, crvec p3, crvec p4, int nw = 3, int nh = 4)
{
	DEF_SCALES(1);
	real w = (p1 - p2).len();
	real h = (p3 - p2).len();
	//int nw = 3;
	//int nh = 4;
	LOOP(i, len, nh)
		vec _p14 = blend(p1, p4, real(i - 1) / len);
	vec p14 = blend(p1, p4, ai);
	vec _p23 = blend(p2, p3, real(i - 1) / len);
	vec p23 = blend(p2, p3, ai);
	if (i > 0)
	{
		LOOP(j, wid, nw)
			if (j > 0)
			{
				vec pp1 = blend(_p14, _p23, real(j - 1) / wid);
				vec pp2 = blend(_p14, _p23, aj);
				vec pp4 = blend(p14, p23, real(j - 1) / wid);
				vec pp3 = blend(p14, p23, aj);
				if (j == 2 && i == 2)
				{
					VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
					color = 1;
					extrudeedgeex(e, s6 * E * E * E, 1 - E * E * E);
					color = rrnd(0, 100) < 5 ? 2 : blendcor(0xFF00ffff, 0xFF0000FF, rrnd());
					face(e);
					color = 0xFFFFFFFF;
				}
				else
				{
					VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
					face(e);
				}
			}
		END
	}
	END
}
// ------------------------------------------------
// 田子窗
void tianwindow(crvec p1, crvec p2, crvec p3, crvec p4, int nw = 3, int nh = 4)
{
	DEF_SCALES(1);
	real w = (p1 - p2).len();
	real h = (p3 - p2).len();
	//int nw = 3;
	//int nh = 4;
	LOOP(i, len, nh)
		vec _p14 = blend(p1, p4, real(i - 1) / len);
	vec p14 = blend(p1, p4, ai);
	vec _p23 = blend(p2, p3, real(i - 1) / len);
	vec p23 = blend(p2, p3, ai);
	if (i > 0)
	{
		LOOP(j, wid, nw)
			if (j > 0)
			{
				vec pp1 = blend(_p14, _p23, real(j - 1) / wid);
				vec pp2 = blend(_p14, _p23, aj);
				vec pp4 = blend(p14, p23, real(j - 1) / wid);
				vec pp3 = blend(p14, p23, aj);
				if ((j == 2 || j == 3) && (i == 2 || i == 3))
				{
					VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
					color = 1;
					extrudeedgeex(e, s6 * E * E * E, 1 - E * E * E);
					color = rrnd(0, 100) < 5 ? 2 : blendcor(0xFF20ffff, 0xFF2000FF, rrnd());
					face(e);
					color = 0xFFFFFFFF;
				}
				else
				{
					VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
					face(e);
				}
			}
		END
	}
	END
}
// ------------------------------------------------
// 门
// ------------------------------------------------
// 方门
void rectdoor(crvec p1, crvec p2, crvec p3, crvec p4)
{
	DEF_SCALES(1);
	real w = (p1 - p2).len();
	real h = (p3 - p2).len();
	int nw = 3;
	int nh = 2;

	LOOP(i, len, nh)
		vec _p14 = blend(p1, p4, real(i - 1) / len);
	vec p14 = blend(p1, p4, ai);
	vec _p23 = blend(p2, p3, real(i - 1) / len);
	vec p23 = blend(p2, p3, ai);
	if (i > 0)
	{
		LOOP(j, wid, nw)
			if (j > 0)
			{
				vec pp1 = blend(_p14, _p23, real(j - 1) / wid);
				vec pp2 = blend(_p14, _p23, aj);
				vec pp4 = blend(p14, p23, real(j - 1) / wid);
				vec pp3 = blend(p14, p23, aj);
				if (j == 2 && (i == 1))
				{
					VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
					//color = 1;
					//unflag(0);
					//VECLIST ee;
					color = blendcor(0xFF800000, 0xFF000000, rrnd());
					//if (ghitdis > USIZE / 2)
					{						
						extrudeedgeex(e, -USIZE / 2, 1 - E * E * E * E * E);
					}
					//drawmask = 0xFFFFFFFF;					
					face(e);
					color = 0xFFFFFFFF;
					gtargets.push_back(getedgecenter(e));
				}
				else
				{
					VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
					face(e);
				}
			}
		END
	}
	END
}
// ------------------------------------------------
// 圆形门
void doublevnumex(VECLIST& e, int num = 0)
{
	VECLIST te;
	int n = num == 0 ? e.size() - 1 : num;

	for (int i = 0; i < n; i++)
	{
		te.PUSH(e[i]);
		if (i == 1 || i == 2)
		{
			te.PUSH(e[i]);
			te.PUSH(e[i]);
		}
	}
	e = te;
}
void roundex(VECLIST& be, real r, int n = 8)
{
	DEF_SCALES(1);
	//renderstate = 2;
	vec o = (be[0] + be[1]) / 2;
	VECLIST e;
	{// 半圆
		vec dx = be[1] - be[0]; dx.norm();
		vec dy = be[2] - be[1]; dy.norm();
		LOOP(i, len, n)
			real angi = ai * PI;	
			vec2 dpi((r)* cos(angi), (r)* sin(angi));
			e.PUSH(o + (dx * dpi.x + dy * dpi.y));
		END
	}
	VECLIST e0;
	e0.push_back(be[1]); e0.push_back(be[2]); e0.push_back(be[3]); e0.push_back(be[0]); closeedge(e0);
	doublevnumex(e0, 4);
	closeedge(e0);
	face(e0, e);
	
	if (ghitdis > USIZE / 2)
	{
		color = 1;// blendcor(0xFF800000, 0xFF000000, rrnd());
		extrudeedgeex(e, -USIZE / 2, 1);
		o += getedgenorm(be) * (-USIZE / 2);
	}	
	color = 0xFF800000;// blendcor(0xFF800000, 0xFF000000, rrnd());	

	face(e, o);
	//color = 0xFFFFFFFF;
}

void rounddoor(crvec p1, crvec p2, crvec p3, crvec p4, int depth = 0)
{
	DEF_SCALES(1);
	real w = (p1 - p2).len();
	real h = (p3 - p2).len();
	int nw = 3;
	int nh = 2;
	LOOP(i, len, nh)
		vec _p14 = blend(p1, p4, real(i - 1) / len);
		vec p14 = blend(p1, p4, ai);
		vec _p23 = blend(p2, p3, real(i - 1) / len);
		vec p23 = blend(p2, p3, ai);
		if (i > 0)
		{
			LOOP(j, wid, nw)
				if (j > 0)
				{
					vec pp1 = blend(_p14, _p23, real(j - 1) / wid);
					vec pp2 = blend(_p14, _p23, aj);
					vec pp4 = blend(p14, p23, real(j - 1) / wid);
					vec pp3 = blend(p14, p23, aj);
					if (j == 2 && (i == 1))
					{
						vec pp14 = blend(pp1, pp4, 2 / 3.0);
						vec pp23 = blend(pp2, pp3, 2 / 3.0);
						{// 上半部分半圆
							VECLIST e; e.PUSH(pp14); e.PUSH(pp23); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
							real sz = (e[0] - e[1]).len();							
							roundex(e, sz * 0.5);
							
						}						
						{// 下半部方形
							VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp23); e.PUSH(pp14); closeedge(e);
							if (ghitdis > USIZE / 2)
							{
								color = 1;
								//unflag(0);
								unflag(2);
								extrudeedgeex(e, -USIZE / 2, 1);
								drawmask = 0xFFFFFFFF;
							}
							color = 0xFF800000;// blendcor(0xFF800000, 0xFF000000, rrnd());
							face(e);
							color = 0xFFFFFFFF;
							gtargets.push_back(getedgecenter(e));
						}
					}
					else
					{
						VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
						face(e);
					}
				}
			END
		}
	END
}
// 圆形窗户
void roundwindow(crvec p1, crvec p2, crvec p3, crvec p4, int depth = 0)
{
	VECLIST e0; e0.push_back(p1); e0.push_back(p2); e0.push_back(p3); e0.push_back(p4); 
	vec n = getedgenorm(e0);
	doublevnumex(e0, 4);
	closeedge(e0);

	real r = MIN((p1 - p2).len(), (p1 - p4).len()) / 4;
	vec o = (p1 + p3) / 2;
	VECLIST e;
	{// 圆
		vec dx = p2 - p1; dx.norm();
		vec dy = p3 - p2; dy.norm();
		LOOP(i, len, 8)
			real angi = ai * 2 * PI + PI;
		vec2 dpi((r)* cos(angi), (r)* sin(angi));
		e.PUSH(o + (dx * dpi.x + dy * dpi.y));
		END
	}
	face(e0, e);
	real d = -USIZE / 2;
	{//
		VECLIST e1;		
		extrudeedge(e, d, e1);
		for (int i = 0; i < e.size() - 1; i++)
		{
			//color = blendcor(0xFF00ffff, 0xFF0000FF, rrnd());
			color = 1;
			if (checkflag(i))
				plane(e[i].p, e[i + 1].p, e1[i + 1].p, e1[i].p, RNDBW);
		}
		VECLIST e2;
		scaleedge(e1, 1 - E2, e2);
		face(e1, e2);
		e = e2;
	}
	// 窗体
	color = blendcor(0xFF00FFFF, 0xFF004080, rrnd());	
	face(e, o + n * d);
	color = 0xFFFFFFFF;
}

// ------------------------------------------------
// 墙壁
// ------------------------------------------------
void wall(crvec p1, crvec p2, crvec p3, crvec p4, int depth = 0)
{
	real s = (p1 - p2).len();
	DEF_SCALES(s);
	
	VECLIST e;e.PUSH(p1);e.PUSH(p2);e.PUSH(p3);e.PUSH(p4);closeedge(e); // wall	
	if(depth == 3)
	{	
		point(p1, 8, 0xFF0000ff);
		point(p2, 8, 0xFF00ffff);
		point(p3, 8, 0xFF00ff00);
		point(p4, 8, 0xFFff0000);
		
		face(e);
		return;
	}
	{			
		real bp1 = rrnd(0.25, 0.75);
		vec p14 = blend(p1, p4, bp1);
		vec p23 = blend(p2, p3, bp1);
		if(rrnd() < 0.5)
		{// up | lower
			VECLIST ee;ee.PUSH(p1);ee.PUSH(p2);ee.PUSH(p23);ee.PUSH(p14);closeedge(ee); // lower
			//face(e, ee);
			extrudeedgeex(ee, s1 / E, 1);				
			window(ee[0], ee[1], ee[2], ee[3]);	
		}
		else
		{// left | right
			real bp2 = rrnd(0.25, 0.75);
			vec p12 = blend(p1, p2, bp2);	
			vec P34 = blend(p4, p3, bp2);				
				
			if(rrnd() < 0.5)
			{
				VECLIST ee;ee.PUSH(p1);ee.PUSH(p12);ee.PUSH(P34);ee.PUSH(p4);closeedge(ee);// left
				//face(e, ee);
				extrudeedgeex(ee, s1, 1);				
				window(ee[0], ee[1], ee[2], ee[3]);	
			}	
			else
			{
				VECLIST ee;ee.PUSH(p2);ee.PUSH(p3);ee.PUSH(P34);ee.PUSH(p12);closeedge(ee);// right
				//face(e, ee);
				extrudeedgeex(ee, s1, 1);				
				window(ee[0], ee[1], ee[2], ee[3]);	
			}			
		}
	}
}

// ------------------------------------------------
// 建筑物
// ------------------------------------------------
void wall2wall(VECLIST& e1, VECLIST& e2)
{
	for(int i = 0; i < 4; i ++)
	{		
		VECLIST te;
		unionedge(e1, e2, i, 2, te);
		
		if(i % 2 == 0)
			face(te);
		else
			wall(te[0], te[1], te[2], te[3]);
	}
}
// ------------------------------------------------
// cube建筑单元
void cube(vec o, vec uy, real s)
{
	DEF_SCALES(s);
	uy.norm();vec vx, vy;v2vxvy(uy, vx, vy);
	VECLIST e; roundedge(e, o, vx, vy, s * 1.414 / 2.0, 4);closeedge(e);
	
	VECLIST e1;
	extrudeedge(e, s, e1);	
	face(e, e1);
	face(e1);
	{//pworkingedge
		unionedge(e, e1, workingedgeindex, 2, *pworkingedge);			
	} 
}
void cube(vec o, real s)
{
	DEF_SCALES(s);
	VECLIST e; roundedge(e, o, vec::UX, vec::UZ, s * 1.414 / 2.0, 4, PI / 4);closeedge(e);
	
	VECLIST e1;
	extrudeedge(e, s, e1);	
	face(e, e1);
	face(e1);
	{//pworkingedge
		unionedge(e, e1, workingedgeindex, 2, *pworkingedge);			
	} 
}
// ------------------------------------------------
// 居民楼
void building(vec o, vec uy, real s)
{	
	DEF_SCALES(s);
	uy.norm();vec vx, vy;v2vxvy(uy, vx, vy);
	VECLIST e; roundedge(e, o, vx, vy, s * E / 2, 4);closeedge(e);
	
	VECLIST e1;
	extrudeedge(e, s * rrnd(0.75, 1.25), e1);	
	face(e, e1);
	for(int i = 0; i < 4; i ++)
	{		
		VECLIST te;
		unionedge(e, e1, i, 2, te);
		wall(te[0], te[1], te[2], te[3]);
	}	
	face(e1);
}

// ------------------------------------------------
// 金字塔型
void pyramid(crvec o, vec uy, real a)
{
	uy.norm();
	real r =  sqrt(6.0) / 4 * a;
	real h =  sqrt(6.0) / 3 * a;
	real rr = sqrt(a * a - h * h);
	vec oo = o - uy * (h * 0.1);	
	vec vx, vz;v2vxvy(uy, vx, vz);

	VECLIST e;
	for(int i = 0; i < 4; i ++)
	{
		real ang = i / 4.0 * 2 * PI;
		e.PUSH(oo + vx * (rr * cos(ang)) + vz * (rr * sin(ang)));
	}
	closeedge(e);
	real step = h / 16;
	FOR(i, len, 8)		
		if(i == 0)
		{
			extrudeedgeex(e, step, E * E);
			VECLIST e1 = e;	extrudeedge(e, h);				
			for(int ii = 0; ii < 4; ii ++)
			{		
				VECLIST te;
				unionedge(e1, e, ii, 2, te);
				wall(te[0], te[1], te[2], te[3]);
			}
		}			
		else
		{
			VECLIST e1;	scaleedge(e, E, e1);face(e, e1);e = e1;
			
			step *= 1 + E * E * E;
			extrudeedgeex(e, step, 1 - E5);
		}
	END
	face(e);	
}

// ------------------------------------------------
// 内舱
void cuberoom(vec o, real s)
{
	vec up = vec::UY;
	vec right = vec::UX;
	vec forward = vec::UZ;
	
	vec lp = o - right * (s / 2);
	vec rp = o + right * (s / 2);
	vec lup = o - right * (s / 4) + up * (s / 2);
	vec rup = o + right * (s / 4) + up * (s / 2);
	
	VECLIST e;e.PUSH(rp);e.PUSH(rup);e.PUSH(lup);e.PUSH(lp);
	closeedge(e);
	face(e);	
	
	// front cube
	VECLIST e1;
	extrudeedge(e, -s / 2, e1);
	scaleedge(e1, 0.1);
	face(e, e1);
	
	VECLIST e2;
	unionedge(e, e1, 3, 2, e2);
	//cubearray(e2[0], e2[1], e2[2], e2[3], rrnd(2, 8), 0);
	//cubeboard(e2);

}

// ------------------------------------------------
// 月球车
// ------------------------------------------------
void wheel(crvec o, crvec up, real s)
{
	DEF_SCALES2(s);
	VECLIST be;
	round(be, o, up, s / 2);
	extrudeedgeex(be, s2, 1);
	face(be, getedgecenter(be));
}
void link(crvec p1, crvec p2, real s)
{
	DEF_SCALES2(s);
	vec v = (p2 - p1);
	real len = v.len();
	v.norm();
	VECLIST be;
	round(be, p1, v, s, 4, PI / 4);
	face(be);
	extrudeedgeex(be, len, 1);
	face(be);	
}
void rover1(vec o, vec up, real s)
{	
	DEF_SCALES2(s);
	up.norm();
	vec vx, vy;
	v2vxvy(up, vx, vy);
	
	{// body
		real sx = s1;
		real sy = s;
		VECLIST be;
		plane(be, o, up, sx, sy);
		closeedge(be);
		extrudeedgeex(be, s2, 1);
		VECLIST e0;
		extrudeedgeex(be, s2, E1, e0);
		unflag(2);
		face(be, e0);
		face(e0);
		{
			VECLIST ee;
			unionedge(be, e0, 2, 2, ee); closeedge(ee);
			extrudeedgeex(ee, s * E8, 1 - E2);
		//	face(ee);
		}
	}		
}

// ------------------------------------------------
// 原型UI
// ------------------------------------------------
void roundedgeex(VECLIST& e, crvec o, crvec vx, crvec vy, real r, int sig, real ang0 = 0)
{
	vertex v;
	CIRCLE2(i, r, sig, ang0)		
		v.p = o + (vx * dpi.x + vy * dpi.y);
		v.uv.x = 0.5f + dpi.x / r;
		v.uv.y = 0.5f + dpi.y / r;
		e.PUSH(v);		
	END
}
void roundedgeex(VECLIST& e, crvec o, vec v, real r, int sig, real ang0 = 0)
{
	v.norm();
	vec dx, dy;
	v2vxvy(v, dx, dy);
	roundedgeex(e, o, dx, dy, r, sig, ang0);
}
void magicCube(crvec o, int dat, real s)
{
	dat = 5;
	real ang0 = g_real / 180 * PI;
	VECLIST e;
	roundedge(e, o, vec::UY, s * 1.25, dat, ang0 + PI / 10);
	color = 0xFFA0A0A0;
	extrudeedgeex(e, s * E3, E3);
	VECLIST e1;
	extrudeedgeex(e, s * E5, 1 - blend(E2, E3, g_real2), e1);
	const int colors[] = { 0xFFFF00FF, 0xFF0000FF, 0xFF00FF00, 0xFFFFFFFF, 0xFF00FFFF };
	for (int i = 0; i < dat; i++)
	{
		VECLIST eo;
		unionedge(e, e1, i, 2, eo); closeedge(eo);
		color = colors[i % dat];
		//LOGMSG("TRI_S: " << gcurtriangle);
		extrudeedgeex(eo, s * E5, 1);
		extrudeedgeex(eo, s * E, 1 + E + E3);
		extrudeedgeex(eo, s * E5, 1);
		face(eo);
		color = 0xFFFFFFFF;
		gtargets.push_back(getedgecenter(eo));
		//LOGMSG("TRI_E: " << gcurtriangle);
	}
	color = 0xFF9DD8EC;
	extrudeedgeex(e1, s * E6, 1 - E1);
	color = 0xFF00FFFF;
	face(e1, getedgecenter(e1));
	color = 0xFFFFFFFF;
}
void magicCubeEx(crvec o, int dat, real s)
{
	dat = 5;	
	VECLIST e;
	roundedge(e, o, vec::UY, s * blend(0.1, 1.25, g_real), dat, PI / 10);
	color = 0xFF004000;
	extrudeedgeex(e, s * E3, E3);
	VECLIST e1;
	extrudeedgeex(e, s * E5, 1 - E2, e1);
	const int colors[] = { 0xFFFF00FF, 0xFF0000FF, 0xFF00FF00, 0xFFFFFFFF, 0xFF00FFFF };
	for (int i = 0; i < dat; i++)
	{
		VECLIST eo;
		unionedge(e, e1, i, 2, eo); closeedge(eo);
		color = colors[i % dat];
		//LOGMSG("TRI_S: " << gcurtriangle);
		extrudeedgeex(eo, s * E5, 1);
		extrudeedgeex(eo, s * E3, 1 + E + E3);
		extrudeedgeex(eo, s * E1, E2);
		face(eo);
		color = 0xFFFFFFFF;
		gtargets.push_back(getedgecenter(eo));
		//LOGMSG("TRI_E: " << gcurtriangle);
	}
	color = 0xFF9DD8EC;
	extrudeedgeex(e1, s * E6, 1 - E1);
	color = 0xFF00FFFF;
	face(e1, getedgecenter(e1));
	color = 0xFFFFFFFF;
}
void magicbox(crvec o, int dat, real s)
{
	//if (dat == 4)
	return magicCube(o, dat, s);

	VECLIST e;
	roundedge(e, o, vec::UY, s, dat, PI / dat);
	color = blendcor(1, 0xFF5A4E6A, 0.5);
	extrudeedgeex(e, s * E6, 1);
	color = 0xFFFFFFFF;
	extrudeedgeex(e, s * E3, 1 - E3);
	VECLIST e1;
	extrudeedgeex(e, -s * E5, 1, e1);	
	for (int i = 0; i < dat; i ++)
	{
		VECLIST eo;
		unionedge(e, e1, i, 2, eo); closeedge(eo);
		extrudeedgeex(eo, s * E8, 1 - E3);	
		color = 0xFF5A4E6A;
		extrudeedgeex(eo, -s * E9 / 2, 1 - E4);
		color = RGB(255, rrnd(100, 255), 0);
		face(eo);
		color = 0xFFFFFFFF;
		gtargets.push_back(getedgecenter(eo));
	}
	color = 0xFF9DD8EC;
	extrudeedgeex(e1, s * E6, 1 - E1);
	color = blendcor(1, 0xFF5A4E6A, 0.75);
	face(e1, getedgecenter(e1));
	color = 0xFFFFFFFF;	
}

// ------------------------------------------------
void linequad(VECLIST& e, crvec o, crvec vx, crvec vy, real r, int sig, real ang0 = 0)
{
	vertex v;
	CIRCLE2(i, r, sig, ang0)
		v.p = o + (vx * dpi.x + vy * dpi.y);
	v.uv.x = 0.5f + dpi.x / r;
	v.uv.y = 0.5f + dpi.y / r;
	e.PUSH(v);
	END
}
void linequad2(crvec p1, crvec p2, vec v, real sz)
{
	v.norm();
	vec dy = v.cross(p2 - p1); dy.norm();
	vec p11 = p1 + dy * (sz / 2);
	vec p12 = p1 - dy * (sz / 2);
	vec p21 = p2 + dy * (sz / 2);
	vec p22 = p2 - dy * (sz / 2);
	VECLIST e;
	e.push_back(p11); e.push_back(p12); e.push_back(p22); e.push_back(p21);
	face(e);
}
// ------------------------------------------------
bool line3d2(vec p1, vec p2, vec cam, real wid)
{
	vec pm = (p1 + p2) / 2;	
	vec n = (cam - pm).normcopy();
	linequad2(p1, p2, -n, wid);
	return true;
}
// ------------------------------------------------
bool line3d(vec p1, vec p2, vec cam, real wid)
{
	VECLIST e;
	vec v12 = p2 - p1;
	roundedge(e, p1, v12, wid / 2, 4);
	face(e);
	extrudeedgeex(e, wid / 2, 2);
	extrudeedgeex(e, v12.len(), 1);
	extrudeedgeex(e, wid / 2, 0.5);
	face(e);

	return true;
}
// ------------------------------------------------
// PM怪
// ------------------------------------------------
// mudbug1
real gtimealpha = 0;

vec bzblend(int ind, vec p1, vec p2, real t, real deta1 = 0, real deta2 = 0)
{	
	const int MAX_WORM = 50;
	static vec bzblend_dp[MAX_WORM][4];
	static bool binit[MAX_WORM] = { 0 };
	if (!binit[ind])
	{
		if(!binit[0])
		for (int ii = 0; ii < 512; ii++)
		for (int jj = 0; jj < 512; jj++)
		{
			rndmap[ii][jj] = rrnd(0, 1);
		}
		binit[ind] = true;
		
		vec v = p2 - p1;
		vec vx, vy;
		v2vxvy(v, vx, vy);
		bzblend_dp[ind][0] = vx * blend(-deta1, deta1, rndmap[ind][1]) + vy * blend(-deta1, deta1, rndmap[ind][2]);
		bzblend_dp[ind][1] = vx * blend(-deta1, deta1, rndmap[ind][3]) + vy * blend(-deta1, deta1, rndmap[ind][4]);
		bzblend_dp[ind][2] = vx * blend(-deta2, deta2, rndmap[ind][5]) + vy * blend(-deta2, deta2, rndmap[ind][6]);
		bzblend_dp[ind][3] = vx * blend(-deta2, deta2, rndmap[ind][7]) + vy * blend(-deta2, deta2, rndmap[ind][8]);
	};

	vec bzblend_cp[4];	
	bzblend_cp[0] = p1;
	bzblend_cp[1] = p1 * 0.75 + p2 * 0.25;
	bzblend_cp[2] = p1 * 0.25 + p2 * 0.75;
	bzblend_cp[3] = p2;

	for (int i = 0; i < 4; i ++)
		bzblend_cp[i] += bzblend_dp[ind][i] * perlin(bzblend_cp[i].x * 10, bzblend_cp[i].y * 10, (gtimealpha + rndmap[ind][18]) * 10);

	return bezier3(bzblend_cp, t);
}
// ------------------------------------------------
void mudbug1(int ind, crvec o, crvec tp, int corners, real s)
{	
	VECLIST e;
	vec p = o;
	int len = 18;
	for (int i = 0; i < len; i++)
	{
		real ai = real(i) / len;
		real t = blend(0.0, 1, ai, 0.5);
		vec np = bzblend(ind, o, tp, t, 0.2, 0.5);
		{
			VECLIST e1;
			if(i == 0)
				roundedge(e1, p, vec::UY, s, corners);
			else
			{
				moveedge(e, (np - p), e1);
				scaleedge(e1, 0.85);
				rotedgebynorm(e1, (np - p));
			}
			if(i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	{// 顶端
		VECLIST e1;
		extrudeedge(e, s * 0.004, e1);
		scaleedge(e1, 2.618);
		face(e, e1);
		gtargets.push_back(getedgecenter(e1));
	}
}
// ------------------------------------------------
// 腔肠动物
void Coelenterate(vec o, vec v0, real s,
int n, 	// 体节数
int aa, // 触角数
int an  // 触角节数
)
{
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = n;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.5);
		vec np = p + (v0 + dp) * (s * 10);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, s, aa);
			}
			else if (i < len)
			{// 身体
				moveedge(e, (np - p), e1);
				float sc = blend(1.35, 0.2, ai, 3.5);
				scaleedge(e1, sc);
				//rotedgebynorm(e1, v0);
			}
			else
			{// 头部与触手
				moveedge(e, (np - p) * 0.1, e1);
				scaleedge(e1, 0.25);
				for (int ii = 0; ii < aa; ii ++)
				{
					VECLIST ee;
					unionedge(e, e1, ii, 2, ee);
					closeedge(ee);
					vec v = getedgenorm(ee);
					for (int jj = 0; jj < an; jj++)
					{						
						v += vec::UY * 0.01;
						real ang = (perlin(p.x * 10, p.y * 40, p.z * 10 + gtimealpha * 8) * 0.5);
						v.rot(ang, vec::UY);
						v.norm();
						VECLIST ee1;
						moveedge(ee, v * (s * 10), ee1);
	
						face(ee, ee1);
						ee = ee1;
					}
				}
			}
			if(i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
}
// ------------------------------------------------
// PM天南星植物
// ------------------------------------------------
// 花形
// -------------------------------------------------------------
void Araceae2(vec o, vec v0, real s,
	int n,
	int aa,
	int an
	)
{	
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = n;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.75);
		vec np = p + (v0 + dp) * (s * 10);
		v0 = v0 + vec::UY * 0.25f; v0.norm();
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, 8 * s, aa);
			}
			else if (i < len)
			{// 身体
				color = 0xFF001000;
				moveedge(e, (np - p), e1);
				float sc = blend(0.9, 1, ai, 3.5);
				scaleedge(e1, sc);
				//rotedgebynorm(e1, v0);
				color = 0xFF008000;
			}
			else
			{// 头部
				color = 0xFF40AAFF;
				moveedge(e, (np - p) * 0.02, e1);
				//scaleedge(e1, 1.2);
				for (int ii = 0; ii < aa; ii++)
				{
					VECLIST ee;
					unionedge(e, e1, ii, 2, ee);
					closeedge(ee);
					vec v = getedgenorm(ee);
					real cvg = rrnd(0.025, 0.75);
					for (int jj = 0; jj < an; jj++)
					{
						real ajj = jj / real(an);
						v -= vec::UY * blend(-0.25, cvg, ajj);
						v.norm();
						VECLIST ee1;
						moveedge(ee, v * (s * 5), ee1);
						float sc = blend(1.5, 0.1, ajj, 1.5);
						scaleedge(ee1, sc);
						face(ee, ee1);
						ee = ee1;
					}
				}
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	//face(e, getedgecenter(e) + vec::UY * (s * 4));
	color = 0xFFFFFFFF;
}
// ------------------------------------------------
// 常见菊类植物
// ------------------------------------------------
void zitengleaf(vec o, vec v, vec n, real len, real w)
{
	vec vside = v.cross(n); vside.norm();
	triang(o, o + v * len - vside * (w / 2), o + v * len + vside * (w / 2));
}
void Asterids(vec p, vec v, real s)
{
	DEF_SCALES2(s);
	VECLIST e;
	vec uz = vec::UZ;
	v.norm(); //v.rot(ang0, vec::UY);
	real cf = rrnd(1.25, 2);
	int len = 25;

	color = 0xFF001000;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		v += vec::UY * 0.1; // 重力
		v.norm();
		vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.75);
		vec np = p + (v + dp) * (s * 0.25);
		//vec np = p + (v)* (s * 0.25);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, v, s * 0.1, 3);
			}
			else
			{// 身体
				moveedge(e, (np - p), e1);
				rotedgebynorm(e1, v);
				scaleedge(e1, 0.92);
			}
			if (ai > 0.5)
			{
				for (int ii = 0; ii < 6; ii++)
				{// 花
					vec dx, dy;
					v2vxvy(v, dx, dy);

					vec vv = dx.rotcopy(rrnd(-PI, PI), v);
					color = blendcor(RGB(255, 73, 64), RGB(255, 0, 0), rrnd());
					zitengleaf(p, -vv, rrnd() < 0.5 ? -v : v, s * blend(1, 0.1, ai), s * 0.25);					
				}
			}
			
			color = 0xFF002010;
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	color = 0xFFFFFFFF;
}
// ------------------------------------------------
// 仙人掌
// ------------------------------------------------
void Araceae1(vec o, vec v0, real s, int depth = 0)
{
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = 16;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.5);
		v0 += vec::UY * 0.2; v0.norm();
		vec np = p + (v0 + dp) * (s * 10);
		{
			color = blendcor(0xFF001000, 0xFF608000, ai);
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, s, 8);
			}
			else
			{// 身体
				moveedge(e, (np - p), e1);
				//float sc = blend(1.35, 0.5, ai, 3.5);
				float sc = blend(1.5, 0.5, ai, 2.5);
				sc *= blend(0.5, 1.35, ai, 0.5);
				scaleedge(e1, sc);
				{// 斑点
					vec pp = e[int(rrnd(0, 7))];
					gtargets.push_back(pp);
				}
			}	
			if (i > 0 && depth < 2 && rrnd() < 0.1)
			{
				vec pp = e[int(rrnd(0, 7))];
				Araceae1(pp, (pp - p).normcopy(), s  * 0.5, depth + 1);
			}
			else
			if (i > len / 2 && depth < 2 && rrnd() < 0.1)
			{
				vec pp = e[int(rrnd(0, 7))];
				Araceae2(pp, (pp - p).normcopy(), s  * 0.25, 8, 4, 8);
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	face(e, getedgecenter(e) + vec::UY * (s * 4));
	color = 0xFFFFFFFF;
}
void Araceae3(vec o, vec v0, real s)
{
	color =	blendcor(0xFF204000, 0xFF605000, rrnd());
	VECLIST e;
	roundedge(e, o, vec::UY, s, 8);
	o += vec::UY * (s * E2);
	for (int i = 0; i < e.size() - 1; i++)
	{
		VECLIST e1;
		e1.push_back(o);
		e1.push_back(e[i]);
		e1.push_back(e[i + 1]);
		closeedge(e1);

		vec v = getedgenorm(e1);
		v.y *= 0.25; v.norm();
		for (int ii = 0; ii <= 8; ii++)
		{
			real aii = ii / real(8);
			v += vec::UY * blend(0, 0.1, aii);
			VECLIST e2;
			moveedge(e1, v * (s * 8), e2);
			float sc = blend(1.8, 0.5, aii, 1);
			scaleedge(e2, sc);
			face(e1, e2);
			e1 = e2;
			face(e1, getedgecenter(e1) + vec::UY * (s * 4));
		}
	}	
	color = 0xFFFFFFFF;
}
void Araceae4(vec o, vec v0, real s)
{
	color = blendcor(0xFF204000, 0xFF605000, rrnd());
	VECLIST e;
	roundedge(e, o, vec::UY, s, 8);
	o += vec::UY * (s * E2);
	for (int i = 0; i < e.size() - 1; i++)
	{
		VECLIST e1;
		e1.push_back(o);
		e1.push_back(e[i]);
		e1.push_back(e[i + 1]);
		closeedge(e1);

		vec v = getedgenorm(e1);
		v.y *= 0.25; v.norm();
		for (int ii = 0; ii <= 8; ii++)
		{
			real aii = ii / real(8);
			v -= vec::UY * blend(0, 0.1, aii);
			VECLIST e2;
			moveedge(e1, v * (s * 8), e2);
			float sc = blend(1.8, 0.5, aii, 1);
			scaleedge(e2, sc);
			face(e1, e2);
			e1 = e2;
			face(e1, getedgecenter(e1) + vec::UY * (s * 4));
		}
	}
	color = 0xFFFFFFFF;
}
// -------------------------------------------------------------
// 蕨类植物
// -------------------------------------------------------------
void curve(vec p, vec v, real s, int depth = 0)
{
	s *= 0.5;
	DEF_SCALES2(s);
	VECLIST e;	
	vec uz = vec::UZ;
	real ang0 = rrnd(-PI / 18, PI / 18);
	uz.rot(ang0, vec::UY);
	v.norm(); v.rot(ang0, vec::UY);
	real cf = rrnd(1.25, 2);
	int len = 32;
	color = blendcor(0xFF002000, 0xFF004020, rrnd());
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec np = p + (v) * (s * 10);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				if(depth == 0)
					roundedge(e1, p, v, s * 2, 6);
				else
					roundedge(e1, p, v, s * 2, 3);
			}
			else
			{// 身体
				moveedge(e, (np - p), e1);
				rotedgebynorm(e1, v);
				real ang = blend(0, PI / 2, ai, cf) * (depth % 2 == 0 ? 1 : -1);
				v.rot(ang, uz);
			}
			if (depth < 1 && i < 2 && rrnd() < 0.1)
			{				
				curve(p, v, s / 2, depth + 1);
			}
			
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}	
}

// ---------------------------------------------------------------------------------
// 互动机械
// ---------------------------------------------------------------------------------
void rover(real s)
{
	DEF_SCALES2(s);	
	vec forword = vec::UZ;
	vec p = vec::CENTER + vec::UY * (s / 8);
	{// sphere body
		VECLIST e;
		roundedge(e, p, forword, s, 16);
		
		int len = 8;
		
		{// 背后
			binvnorm = true;
			VECLIST e1 = e;
			for (int i = 0; i <= len; i++)
			{
				real ai = i / real(len);
				real d = s / len;
				real ss = blend(1, 0.1, ai, 2);
				extrudeedgeex(e1, -d, ss);
			}
			binvnorm = false;
		}
		
		{// 前方
			VECLIST e1 = e;
			for (int i = 0; i < len - 3; i++)
			{
				real ai = i / real(len);
				real d = s / len;
				real ss = blend(1, 0.1, ai, 2);
				extrudeedgeex(e1, d, ss);
			}
			extrudeedgeex(e1, s / 8, 1);			
			{
				//scaleedgeex(e1, 0.8);
				VECLIST e2;
				scaleedge(e1, 0.8, e2);
				for (int i = 0; i < e1.size() - 1; i++)
				{
					if (i % 2 == 0)
					{						
						VECLIST ee; ee.push_back(e1[i].p); ee.push_back(e1[i + 1].p); ee.push_back(e2[i + 1].p); ee.push_back(e2[i].p); closeedge(ee);
						extrudeedgeex(ee, s / 58, 0.75);
						color = 0xFF00FFFF;
						face(ee);
						color = 0xFFFFFFFF;
					}
					else
						plane(e1[i].p, e1[i + 1].p, e2[i + 1].p, e2[i].p, RNDBW);					
				}
			}
		}
	}
}
// ---------------------------------------------------------------------------------
// 杆子, 动态部分
void stem1(vec o, vec v, real len, real r)
{
	VECLIST e;
	roundedge(e, o, v, r, 4);
	face(e);
	extrudeedgeex(e, len, 1);
	face(e);
}
void stem2(vec p1, vec p2, real r)
{
	vec v = (p2 - p1);
	real len = v.len();
	if(len > 0)
		v /= len;

	VECLIST e;
	roundedge(e, p1, v, r, 4);
	face(e);
	extrudeedgeex(e, len, 1);
	face(e);
}
// ---------------------------------------------------------------------------------
// 抓手
void paw(vec p1, vec p2, real r, real talpha)
{
	vec v = (p2 - p1);
	real len = v.len();
	if (len > 0)
		v /= len;

	vec dx, dy;
	v2vxvy(v, dx, dy);

	VECLIST e;
	roundedge(e, p1, dx, dy, r, 4);
	p1 += v * blend(r * E1, r * E2, talpha);
	face(e, p1);
	for (int i = 0; i < 4; i += 2)
	{
		VECLIST e1;
		e1.push_back(p1);
		e1.push_back(e[i]);
		e1.push_back(e[i + 1]);
		closeedge(e1);

		{// 抓指头
			VECLIST e2;
			vec n = getedgenorm(e1);
			moveedge(e1, n * (len * 0.4), e2);
			rotedgebynorm(e2, v, dx);
			face(e1, e2);
			vec pp = getedgecenter(e2);
			vec tp = blend(pp + v * (len * 0.6), p2, 0.25 + talpha);
			face(e2, tp);
		}		
	}
}
void paw(VECLIST& be, vec p1, vec p2, real r, real talpha)
{
	vec v = (p2 - p1);
	real len = v.len();
	if (len > 0)
		v /= len;

	vec dx, dy;
	v2vxvy(v, dx, dy);

	VECLIST e;
	roundedge(e, p1, dx, dy, r, 4);
	face(be, e);
	p1 += v * blend(r * E1, r * E2, talpha);
	face(e, p1);
	for (int i = 0; i < 4; i += 2)
	{
		VECLIST e1;
		e1.push_back(p1);
		e1.push_back(e[i]);
		e1.push_back(e[i + 1]);
		closeedge(e1);

		{// 抓指头
			VECLIST e2;
			vec n = getedgenorm(e1);
			moveedge(e1, n * (len * 0.4), e2);
			rotedgebynorm(e2, v, dx);
			face(e1, e2);
			vec pp = getedgecenter(e2);
			vec tp = blend(pp + v * (len * 0.6), p2, 0.5 + talpha);
			face(e2, tp);
		}
	}
}
// ------------------------------------------------
// 机械手臂
// ------------------------------------------------
void robotarm(vec o, vec v1, vec v2, real s, real len1, real len2)
{
	v1.norm(); v2.norm();
	VECLIST e;
	roundedge(e, o, v1, s, 8);
	{// 基套
		VECLIST e1;
		moveedge(e, v2 * len1, e1);
		rotedgebynorm(e1, v2);
		face(e, e1);
		extrudeedgeex(e1, s * E5, 0.25);
		e = e1;
	}
	{// 杆
		extrudeedgeex(e, len2, 1 - E3);
	}

	//vec pp = getedgecenter(e);
	//paw(e, pp, pp + v2 * (len1 / 4), 0.01, gtimealpha);

	{// 爪
		VECLIST e1, e2;
		extrudeedgeex(e, s, 4, e1);
		face(e, e1);
		extrudeedgeex(e1, s / 4, 0.25, e2);
		face(e1, e2);
		{// 左侧
			VECLIST eo, eo2;
			unionedge(e1, e2, 0, 2, eo); closeedge(eo);
			extrudeedgeex(eo, len1 * 0.1, 1 - E3);
			vec n = getedgenorm(eo);
			moveedge(eo, blend(n, v2, 0.75) * (len1 * 0.35), eo2);
			scaleedge(eo2, 0.1f);
			face(eo, eo2);
		}
		{// 右侧
			VECLIST eo, eo2;
			unionedge(e1, e2, 4, 2, eo); closeedge(eo);
			extrudeedgeex(eo, len1 * 0.1, 1 - E3);
			vec n = getedgenorm(eo);
			moveedge(eo, blend(n, v2, 0.75) * (len1 * 0.35), eo2);
			scaleedge(eo2, 0.1f);
			face(eo, eo2);
		}
	}
}

// -------------------------------------------------
// 建筑物
// -------------------------------------------------
// 亭子
void tingzi(vec o, vec uy, real s)
{
	DEF_SCALES(s)	
	uy.norm();vec vx, vy;v2vxvy(uy, vx, vy);
	
	FOR(i, len, 4)
		real ang = ai * 2 * PI;
		vec oo = o + (vx * cos(ang) + vy * sin(ang)) * s3;
		zhuzi(oo, uy, s);
	END
	jinzita(o + uy * (s2), uy, s2 * E);
}
// ------------------------------------------------
// 圆台
vec yuantai(vec o, vec uy, real s)
{
	DEF_SCALES(s);
	uy.norm();vec vx, vy;v2vxvy(uy, vx, vy);
	VECLIST e; roundedge(e, o, vx, vy, s2 * E, 16);closeedge(e);
	
	extrudeedgeex(e, s4 * E, 1 - E * E * E * E);
	extrudeedgeex(e, -s6, 1 - E * E * E * E * E * E * E * E);
	face(e);
	return getedgecenter(e);
}

// -------------------------------------------------
// 塔
// ------------------------------------------------
void luo(vec o, vec uy, real s)
{	//renderstate = 2;

	DEF_SCALES(s);	
	int len = 100;
	for(int i = 0; i < len; i ++)
	{
		real ai = i / real(len);
		real ang = blend(0, PI * 8, ai);
		
		{			
			real h0 = blend(0, s1 * E, (ang - PI * 2) / (PI * 8));
			real h1 = blend(0, s1 * E, ai);			
			real r = blend(1, 0.1, ai) * s2;
			{// 上下的城墙
				static vec lp0;
				static vec lp1;
				real rr = r;				
				vec dp0(rr * cos(ang), h0, rr * sin(ang));
				vec dp1(rr * cos(ang), h1, rr * sin(ang));
				vec p0 = o + dp0;				
				vec p1 = o + dp1;
				if(i != 0)
				{				
					//line3d(p0, p1);	
					VECLIST e;e.PUSH(p1);e.PUSH(p0);e.PUSH(lp0);e.PUSH(lp1);closeedge(e);
					face(e);
				}
				lp0 = p0;
				lp1 = p1;				
			}
			{// 甬道
				static vec lp0;
				static vec lp1;
				real r0 = blend(1, 0.1, (ang + PI * 2) / (PI * 8)) * s2;
				real rr = r;				
				vec dp0(r0 * cos(ang), h1, r0 * sin(ang));
				vec dp1(rr * cos(ang), h1, rr * sin(ang));
				vec p0 = o + dp0;				
				vec p1 = o + dp1;
				if(i != 0)
				{				
					//line3d(p1, p0);	
					//line3d(p0, lp0);	
					//line3d(lp0, lp1);
					//VECLIST e;e.PUSH(p1);e.PUSH(lp1);e.PUSH(lp0);e.PUSH(p0);invedge(e);closeedge(e);				
					//face(e);
					int num = (lp0 - lp1).len() / 0.01;
					staris(lp1, lp0, p1, p0, num);
				}
				lp0 = p0;
				lp1 = p1;				
			}			
		}		
					
	}
}
// ------------------------------------------------
// 圆顶
void yuanding2(vec o, vec uy, real s)
{
	DEF_SCALES(s);
	uy.norm();vec vx, vy;v2vxvy(uy, vx, vy);
	VECLIST oe;
	FOR(i, len0, 8)
		real r = blend(1, 0.1, ai, 2) * s1;
		real h = blend(0, 1, ai, 0.5) * s1 * 2;
		VECLIST e;
		FOR(ii, len1, 8)
			real ang = aii * 2 * PI;
			vec vv = vx * (r * cos(ang)) + vy * (r * sin(ang)) + uy * h;
			vec p = o + vv;
			e.PUSH(p);
			pixel(p, 0xffFFFFFF);
		END
		closeedge(e);
		if(i > 0)
			face(oe, e);
		oe = e;
	END
}
void yuanding(vec o, vec uy, real s)
{
	gcommonvertex = true;
	DEF_SCALES(s);
	VECLIST e;
	roundedge(e, o, uy, s1, 16);

	int len = 8;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		real d = 1.5 * s1 / len;
		real ss = i == 0 ? 1.2 : blend(1, 0.1, ai, 2);

		extrudeedgeex(e, d, ss);
	}
	gcommonvertex = false;
}
void yuanzhuding(crvec p1, crvec p2, crvec p3, crvec p4)
{
	VECLIST e;
	e.PUSH(p1); e.PUSH(p2); e.PUSH(p3); e.PUSH(p4);
	extrudeedgeex(e, 0.5, 1);
}
// ------------------------------------------------
// 墙壁
// ------------------------------------------------
// 分割
int wall1_rndcor()
{
	int lum = rrnd(200, 255);
	return RGB(lum, lum, 255);
}
real fixedlen(real len)
{
	len = int(len / USIZE) * USIZE;
	return len == 0 ? USIZE : len;
}
real rndlen(real len)
{
	int times = int(rrnd(0, 5));
	for (int i = 0; i < times; i++)
	{
		len *= E;
	}
	return fixedlen(len);
}
bool wallrnd(crvec p1, crvec p2, crvec p3, crvec p4, int depth = 0)
{
	real a12 = rrnd(0, 1);
	real a14 = rrnd(0, 1);
	bool boper = false;

	VECLIST e; e.PUSH(p1); e.PUSH(p2); e.PUSH(p3); e.PUSH(p4); closeedge(e); // wall
	{
		while (abs(a14 - 0.5) > abs(a12 - 0.5))
		{// up | lower	
			real bp1 = a14;
			{// 调整分割点
				real len0 = (p1 - p4).len();
				if (len0 < 1.98 * USIZE)
				{// 太小					
					break;
				}
				else
				{
					real ma = USIZE / len0;
					bp1 = bp1 < 0.5 ? int(bp1 / ma) * ma + ma : int(bp1 / ma) * ma;
					if (bp1 <= 0 || bp1 >= 1.0)
					{// 默认操作						
						break;
					}
				}
			}
			vec p14 = blend(p1, p4, bp1);
			vec p23 = blend(p2, p3, bp1);
			if (a14 > 0.5)
			{
				{
					color = wall1_rndcor();
					VECLIST ee; ee.PUSH(p14); ee.PUSH(p23); ee.PUSH(p3); ee.PUSH(p4); closeedge(ee); // upper
					extrudeedgeex(ee, rndlen((p1 - p2).len()), 1);
					face(ee);
				}

				{// 剩下的部分
					color = wall1_rndcor();
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p2); ee.PUSH(p23); ee.PUSH(p14); closeedge(ee);
					face(ee);
				}
			}
			else
			{
				{
					color = wall1_rndcor();
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p2); ee.PUSH(p23); ee.PUSH(p14); closeedge(ee); // lower
					extrudeedgeex(ee, rndlen((p1 - p2).len()), 1);
					face(ee);
				}
				{// 剩下的部分
					color = wall1_rndcor();
					VECLIST ee; ee.PUSH(p14); ee.PUSH(p23); ee.PUSH(p3); ee.PUSH(p4); closeedge(ee);
					face(ee);
				}
			}
			boper = true;
			break;
		}

		while (!boper)
		{// left | right
			real bp2 = a12;
			{// 调整分割点
				real len0 = (p1 - p2).len();
				if (len0 < 1.98 * USIZE)
				{// 太小					
					break;
				}
				else
				{
					real ma = USIZE / len0;
					bp2 = bp2 < 0.5 ? int(a12 / ma) * ma + ma : int(bp2 / ma) * ma;
					if (bp2 <= 0 || bp2 >= 1.0)
					{// 默认操作						
						break;
					}
				}
			}
			vec p12 = blend(p1, p2, bp2);
			vec P34 = blend(p4, p3, bp2);

			if (a12 < 0.5)
			{
				{
					color = wall1_rndcor();
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p12); ee.PUSH(P34); ee.PUSH(p4); closeedge(ee);// left
					extrudeedgeex(ee, rndlen((p4 - p1).len()), 1);
					face(ee);
				}
				{// 剩下的部分
					color = wall1_rndcor();
					VECLIST ee; ee.PUSH(p12); ee.PUSH(p2); ee.PUSH(p3); ee.PUSH(P34); closeedge(ee);
					face(ee);
				}
			}
			else
			{
				{
					color = wall1_rndcor();
					VECLIST ee; ee.PUSH(p2); ee.PUSH(p3); ee.PUSH(P34); ee.PUSH(p12); closeedge(ee);// right
					extrudeedgeex(ee, rndlen((p4 - p1).len()), 1);
					face(ee);
				}
				{// 剩下的部分
					color = wall1_rndcor();
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p12); ee.PUSH(P34); ee.PUSH(p4); closeedge(ee);
					face(ee);
				}
			}
			boper = true;
			break;
		}
	}
	return boper;
}
int g_manualside = 0; // 0 defalut 1 up|lower 2 left|right
bool wall1(crvec p1, crvec p2, crvec p3, crvec p4, real a12, real a14, bool preview, int depth)
{
	if (g_manualside == 0)
	{
		if (g_vec.dot((p4 - p1).normcopy()) > g_vec.dot((p2 - p1).normcopy()))
		{
			g_manualside = 1;
		}
		else
		{
			g_manualside = 2;
		}
	}
	//logmessage << "\ngcamdis " << gcamdis;
	bool boper = false;	
	VECLIST e; e.PUSH(p1); e.PUSH(p2); e.PUSH(p3); e.PUSH(p4); closeedge(e); // wall
	{
		while(g_manualside == 1 || g_manualside == 0 && abs(a14 - 0.5) > abs(a12 - 0.5))
		{// up | lower	
			real bp1 = a14;
			{// 调整分割点
				real len0 = (p1 - p4).len();				
				if (len0 < 1.98 * USIZE)
				{// 太小					
					break;				
				}
				else
				{	
					// 优先0.5等分	
					/*if(gcamdis > len0 * 4 + 10 * USIZE)
					{					
						if (bp1 > 0.2 && bp1 < 0.8)
							bp1 = 0.5;
					}*/
					/*else if (gcamdis > len0 * 1)
					{
						if (bp1 > 0.4 && bp1 < 0.6)
							bp1 = 0.5;
					}*/
					//if (bp1 != 0.5)
					{
						real ma = USIZE / len0;
						//bp1 = bp1 < 0.5 ? int(bp1 / ma) * ma + ma : int(bp1 / ma) * ma;
						bp1 = int(bp1 / ma + 0.5) * ma; 
						if (bp1 <= 0) bp1 = ma;
						if (bp1 >= 1) bp1 = 1 - ma;
						clipA = int(bp1 / ma);
						clipB = int(len0 / USIZE + 0.5) - clipA;
					}
				}
			}			
			vec p14 = blend(p1, p4, bp1);
			vec p23 = blend(p2, p3, bp1);
			if (a14 > 0.5)
			{
				{			
					color = preview ? 0xFFFF0000 : wall1_rndcor();
					VECLIST ee; ee.PUSH(p14); ee.PUSH(p23); ee.PUSH(p3); ee.PUSH(p4); closeedge(ee); // upper
					//extrudeedgeex(ee, MINSTEP, 1);
					face(ee);
				}

				{// 剩下的部分
					color = preview ? 0xFF00FF00 : wall1_rndcor();
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p2); ee.PUSH(p23); ee.PUSH(p14); closeedge(ee);
					face(ee);
				}
			}
			else
			{
				{
					color = preview ? 0xFFFF00FF : wall1_rndcor();
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p2); ee.PUSH(p23); ee.PUSH(p14); closeedge(ee); // lower
					//extrudeedgeex(ee, MINSTEP, 1);
					face(ee);
				}
				{// 剩下的部分
					color = preview ? 0xFF0000FF : wall1_rndcor();
					VECLIST ee; ee.PUSH(p14); ee.PUSH(p23); ee.PUSH(p3); ee.PUSH(p4); closeedge(ee);
					face(ee);
				}
			}
			boper = true;
			g_manualside = 1;
			break;
		}	
		
		while(!boper)
		{// left | right
			real bp2 = a12;
			{// 调整分割点
				real len0 = (p1 - p2).len();
				if (len0 < 1.98 * USIZE)
				{// 太小					
					break;
				}
				else
				{
					//if (gcamdis > len0 * 4 + 10 * USIZE)
					//{// 优先0.5等分
					//	if (bp2 > 0.2 && bp2 < 0.8)
					//		bp2 = 0.5;
					//}
					/*else if (gcamdis > len0 * 1)
					{
						if (bp2 > 0.4 && bp2 < 0.6)
							bp2 = 0.5;
					}*/
					//if (bp2 != 0.5)
					{
						real ma = USIZE / len0;
						//bp2 = bp2 < 0.5 ? int(a12 / ma) * ma + ma : int(bp2 / ma) * ma;

						bp2 = int(bp2 / ma + 0.5) * ma;
						if (bp2 <= 0) bp2 = ma;
						if (bp2 >= 1) bp2 = 1 - ma;
						clipA = int(bp2 / ma);
						clipB = int(len0 / USIZE + 0.5) - clipA;

					}
				}
			}			
			vec p12 = blend(p1, p2, bp2);
			vec P34 = blend(p4, p3, bp2);
			if (a12 < 0.5)
			{
				{
					color = preview ? 0xFFFF00FF : wall1_rndcor();
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p12); ee.PUSH(P34); ee.PUSH(p4); closeedge(ee);// left
					//extrudeedgeex(ee, MINSTEP, 1);
					face(ee);
				}
				{// 剩下的部分
					color = preview ? 0xFFFFFF00 : wall1_rndcor();
					VECLIST ee; ee.PUSH(p12); ee.PUSH(p2); ee.PUSH(p3); ee.PUSH(P34); closeedge(ee);
					face(ee);
				}
			}
			else
			{
				{
					color = preview ? 0xFF00FF00 : wall1_rndcor();
					VECLIST ee; ee.PUSH(p2); ee.PUSH(p3); ee.PUSH(P34); ee.PUSH(p12); closeedge(ee);// right
					//extrudeedgeex(ee, MINSTEP, 1);
					face(ee);
				}
				{// 剩下的部分
					color = preview ? 0xFF0000FF : wall1_rndcor();
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p12); ee.PUSH(P34); ee.PUSH(p4); closeedge(ee);
					face(ee);
				}
			}
			boper = true;
			g_manualside = 2;
			break;
		}		
	}
	return boper;
}
extern "C"
{
	void EXPORT_API setmanualside(int side)
	{
		g_manualside = side;
	}
	int EXPORT_API getmanualside()
	{
		return g_manualside;
	}
	void EXPORT_API resetmanualside()
	{
		g_manualside = 0;
	}
}

// ------------------------------------------------
// 角凸
int wall2(crvec p1, crvec p2, crvec p3, crvec p4, real a12, real a14, int depth = 0)
{
	bool boper = false;
	int lum = rrnd(200, 255);
	color = RGB(lum, lum, 255);
	{// 调整对齐,1/3		
		if (a12 < 0.5)
			a12 = 0.33;
		else
			a12 = 0.7;
		if (a14 < 0.5)
			a14 = 0.33;
		else
			a14 = 0.7;
	}
	VECLIST e; e.PUSH(p1); e.PUSH(p2); e.PUSH(p3); e.PUSH(p4); closeedge(e); // wall
	{
		while (depth == 0)
		{// up | lower	
			real bp1 = a14;
			{// 调整分割点
				real len0 = (p1 - p4).len();
				if (len0 < 1.8 * USIZE)
				{// 太小					
					break;
				}
				else
				{
					real ma = USIZE / len0;
					bp1 = bp1 < 0.5 ? int(bp1 / ma) * ma + ma : int(bp1 / ma) * ma;
					if (bp1 <= 0 || bp1 >= 1.0)
					{// 默认操作						
						break;
					}
				}
			}
			vec p14 = blend(p1, p4, bp1);
			vec p23 = blend(p2, p3, bp1);
			if (a14 > 0.5)
			{
				{
					VECLIST ee; ee.PUSH(p14); ee.PUSH(p23); ee.PUSH(p3); ee.PUSH(p4); closeedge(ee); // upper
					{
						wall2(ee[0].p, ee[1].p, ee[2].p, ee[3].p, a12, -1, 1);
					}
				}

				//{// 剩下的部分
				//	color = 0xFFFFFFFF;
				//	VECLIST ee; ee.PUSH(p1); ee.PUSH(p2); ee.PUSH(p23); ee.PUSH(p14); closeedge(ee);
				//	face(ee);
				//}
			}
			else
			{
				{
					VECLIST ee; ee.PUSH(p1); ee.PUSH(p2); ee.PUSH(p23); ee.PUSH(p14); closeedge(ee); // lower					
					{
						wall2(ee[0].p, ee[1].p, ee[2].p, ee[3].p, a12, -1, 1);
					}
				}
				//{// 剩下的部分
				//	color = 0xFFFFFFFF;
				//	VECLIST ee; ee.PUSH(p14); ee.PUSH(p23); ee.PUSH(p3); ee.PUSH(p4); closeedge(ee);
				//	face(ee);
				//}
			}
			boper = true;
			break;
		}

		while (!boper || depth == 1)
		{// left | right
			real bp2 = a12;
			{// 调整分割点
				real len0 = (p1 - p2).len();
				if (len0 < 1.8 * USIZE)
				{// 太小					
					break;
				}
				else
				{
					real ma = USIZE / len0;
					bp2 = bp2 < 0.5 ? int(a12 / ma) * ma + ma : int(bp2 / ma) * ma;
					if (bp2 <= 0 || bp2 >= 1.0)
					{// 默认操作						
						break;
					}
				}
			}
			vec p12 = blend(p1, p2, bp2);
			vec P34 = blend(p4, p3, bp2);

			if (a12 < 0.5)
			{
				VECLIST ee; ee.PUSH(p1); ee.PUSH(p12); ee.PUSH(P34); ee.PUSH(p4); closeedge(ee);// left
				extrudeedgeex(ee, USIZE, 1);
				face(ee);
				//{// 剩下的部分
				//	color = 0xFFFFFFFF;
				//	VECLIST ee; ee.PUSH(p12); ee.PUSH(p2); ee.PUSH(p3); ee.PUSH(P34); closeedge(ee);
				//	face(ee);
				//}
			}
			else
			{
				VECLIST ee; ee.PUSH(p2); ee.PUSH(p3); ee.PUSH(P34); ee.PUSH(p12); closeedge(ee);// right
				extrudeedgeex(ee, USIZE, 1);
				face(ee);
				//{// 剩下的部分
				//	color = 0xFFFFFFFF;
				//	VECLIST ee; ee.PUSH(p1); ee.PUSH(p12); ee.PUSH(P34); ee.PUSH(p4); closeedge(ee);
				//	face(ee);
				//}
			}
			boper = true;
			break;
		}		
	}
	return boper ? 2 : 0;
}
// ------------------------------------------------
// 中凸
int wall3(crvec p1, crvec p2, crvec p3, crvec p4, int depth = 0)
{
	DEF_SCALES(1);
	real w = (p1 - p2).len();
	real h = (p3 - p2).len();
	int nw = 3;
	int nh = 3;

	LOOP(i, len, nh)
		vec _p14 = blend(p1, p4, real(i - 1) / len);
	vec p14 = blend(p1, p4, ai);
	vec _p23 = blend(p2, p3, real(i - 1) / len);
	vec p23 = blend(p2, p3, ai);
	if (i > 0)
	{
		LOOP(j, wid, nw)
			if (j > 0)
			{
				vec pp1 = blend(_p14, _p23, real(j - 1) / wid);
				vec pp2 = blend(_p14, _p23, aj);
				vec pp4 = blend(p14, p23, real(j - 1) / wid);
				vec pp3 = blend(p14, p23, aj);
				if (j == 2 && i == 2)
				{
					int lum = rrnd(200, 255);
					color = RGB(lum, lum, 255);
					VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);	
					extrudeedgeex(e, USIZE, 1);
					face(e);
				}
				/*{
					int lum = rrnd(200, 255);
					color = RGB(lum, lum, 255);
					VECLIST e; e.PUSH(pp1); e.PUSH(pp2); e.PUSH(pp3); e.PUSH(pp4); closeedge(e);
					face(e);
				}*/
			}
		END
	}
	END
	color = 0xFFFFFFFF;
	return 2;
}
// ------------------------------------------------
void roadlight(vec o, vec v, real s)
{
	DEF_SCALES2(s);
	VECLIST e;
	v.norm();
	vec uz = vec::UZ;
	//uz.rot(rrnd(0, PI), vec::UY);
	vec p = o;
	int len = 16;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec np = p + (v)* (s * 10);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, s, 8);
			}
			else
			{// 身体
				moveedge(e, (np - p), e1);
				//float sc = blend(1.35, 0.5, ai, 3.5);				
				//scaleedge(e1, sc);				
				rotedgebynorm(e1, v);
				v.rot(blend(0, PI / 3, ai, 4), uz);
			}
			if (i != 0)
			{
				if(i < len)
				{
					face(e, e1);
				}
				else
				{
					scaleedge(e1, 1 / E3);
					face(e, e1);
					gtargets.push_back(blend(getedgecenter(e), getedgecenter(e1), 0.5));
				}
				
			}
			e = e1;
		}
		p = np;
	}
}
// ------------------------------------------------
// 栅栏
void column(crvec p1, crvec p2, crvec up)
{
	vec v = up.cross(p2 - p1); v.norm();

	real flen = (p1 - p2).len();
	real fhgt = flen * (E4);
	real us = USIZE / 6;
	int len = flen / us;
	len = int(len / 12) * 12 + 1;
	us = flen / len;
	for (int i = 0; i < len; i++)
	{
		if (i % 12 == 0)
		{
			real ai = i / real(len);
			vec p = blend(p1, p2, ai);

			VECLIST e; e.push_back(p); e.push_back(p + v * us); e.push_back(p + v * us + up * fhgt); e.push_back(p + up * fhgt); closeedge(e);
			face(e, true);
			extrudeedgeex(e, us, 1);
			face(e);
		}
	}
	{// title
		vec pp1 = p1 + up * fhgt;
		VECLIST e; e.push_back(pp1); e.push_back(pp1 + v * us); e.push_back(pp1 + v * us + up * us); e.push_back(pp1 + up * us); closeedge(e);
		face(e, true);
		extrudeedgeex(e, flen, 1);
		face(e);
	}
}

// -------------------------------------------------------------
// 能生长的植物
// -------------------------------------------------------------
void growing_plant(vec o, vec v0, real s, real t)
{
	if (t == 0)
		return;

	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = 16 * t;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10) * 0.5);
		vec np = p + (v0 + dp) * (s * 10);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, s, 8);
			}
			else
			{// 身体
				moveedge(e, (np - p), e1);
				float sc = blend(1.35, 0.5, ai, 3.5);
				sc *= blend(0.5, 1.35, ai, 0.5);
				scaleedge(e1, sc);
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	vec top = getedgecenter(e) + vec::UY * (s * 1);
	face(e, top);
	if (len == 16)
	{// 成熟后顶端位置形成挂点
		gtargets.push_back(top);
	}
}

// -------------------------------------------------------------
// 竹子-〉芦苇
// -------------------------------------------------------------
void bamboo(vec o, vec v0, real s, int len0 = 10, int depth = 0)
{
	color = 0xFF00FFFF;
	s *= 0.5;
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = len0;
	real dpdeta = rrnd(0, 172.862);
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);

		vec np;
		if (depth == 1)
		{
			vec dp = -vec::UY * 0.1;
			v0 = (v0 + dp).normcopy();
			np = p + (v0) * (s * blend(20, 25, ai, 1));
		}
		else
		{
			vec dp = vec::UX * ((-0.5 + perlin(p.x * 10, p.y * 10, p.z * 10 + dpdeta)) * 0.25) + vec::UZ * ((-0.5 + perlin(p.x * 10 + dpdeta, p.y * 10, p.z * 10)) * 0.25);
			np = p + (v0 + dp - vec::UY * 0.1) * (s * blend(18, 100, ai, 4));
		}		
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				color = 0xFF00FFFF;
				roundedge(e1, p, vec::UY, s, 3);				
			}
			else
			{// 身体
				color = 0xFF00A0A0;
				real dlen = (np - p).len();
				vec gv = (np - p).normcopy();
				if(depth == 0)
				{// 竹节
					VECLIST e2;
					scaleedge(e, 1 / E, e2);					
					moveedge(e2, gv * (USIZE * 0.25));
					face(e, e2); e = e2;	
					scaleedge(e, E);
					moveedge(e, gv * (USIZE * 0.25));
					face(e2, e);
					moveedge(e, np - getedgecenter(e), e1);					
				}
				else
				{
					moveedge(e, gv * (dlen), e1);
				}
				if (i == len)
				{// 顶部					
					if (depth == 0)
					{		
						vec top = np + gv * (dlen * 0.5);
						face(e1, top);
						for (int ii = 0; ii < 18; ii++)
						{
							real aii = ii / 18.0;
							vec vv = blend(gv, vec(rrnd(-1, 1), rrnd(0.5, 1), rrnd(-1, 1)).normcopy(), 0.5);
							bamboo(blend(np, top, aii), vv.normcopy(), s * blend(0.5, 0.1, aii), blend(1, 0.5, aii) * 10, 1);
						}
					}
					else
					{
						gtargets.push_back(np);
					}
				}
				if(depth == 0 && i > len * 0.25)
				{// 叶子
					int ln = rrnd(2, 4);
					for (int ii = 0; ii < ln; ii++)
					{
						vec lvpp = p + vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)).normcopy() * (s * 2.5);
						vec lvv = blend(gv, -vec::UX, rrnd(0.65, 1)); lvv.norm();
						lvv.rot(rrnd(-PI / 4, PI / 4), gv);						
						vec lvpp3 = lvpp + lvv * (s * rrnd(58, 108));
						vec vside = lvv.cross(rrnd() < 0.5 ? -vec::UZ : vec::UZ); vside.norm();
						color = blendcor(0xFF001000, 0xFF004010, rrnd());
						triang(lvpp - vside * (s * 3), lvpp + vside * (s * 3), lvpp3);
						color = 0xFF00A0A0;
					}
				}
			}
			color = 0xFF00A0A0;
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	color = 0xFFFFFFFF;
}
// -------------------------------------------------------------
// 蒲公英
// -------------------------------------------------------------
void dandelion(vec p, vec v, real s, int depth = 0)
{
	real r = rrnd(0.25, 1) * s;
	real h = rrnd(10, 38) * s;
	
	// 底部
	VECLIST e; roundedge(e, p, v, r, 3, rrnd(0, PI)); closeedge(e);
	if(depth  > 0)
	{
		vec pp = p + vec::UY * h;
		face(e, pp);
		gtargets.push_back(pp);
	}
	else if(depth == 0)
	{
		extrudeedgeex(e, h, 0.1);
		p = getedgecenter(e);
		for (int i = 0; i < 8; i++)
		{
			vec vv = vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1));
			dandelion(p, vv.normcopy(), s * 0.25);
		}
	}
}
// -------------------------------------------------------------
// 禾木科,麦穗状
// -------------------------------------------------------------
void grassy(vec p, vec v, real s, int depth = 0)
{
	real r = rrnd(0.25, 1) * s;
	real h = rrnd(10, 38) * s;

	// 底部
	VECLIST e; roundedge(e, p, v, r, 3, rrnd(0, PI)); closeedge(e);
	if (depth  > 0)
	{
		vec pp = p + vec::UY * h;
		face(e, pp);
		gtargets.push_back(pp);
	}
	else if (depth == 0)
	{
		vec vv1 = v.rotcopy(-PI / 4, vec::UZ);
		vec vv2 = v.rotcopy(PI / 4, vec::UZ);
		int len = 8;
		for (int i = 0; i < len; i++)
		{
			real ai = i / real(len);
			extrudeedgeex(e, h, 0.1);
			p = getedgecenter(e);

			grassy(p, vv1.normcopy(), s * 0.25, 1);
			grassy(p, vv2.normcopy(), s * 0.25, 1);
		}
	}	
}
// -------------------------------------------------------------
// 诸葛菜
void grassArt(vec o, vec up, real s)
{		
	int n = rrnd(12, 32);
	for (int i = 0; i < n; i++)
	{
		vec p = o + vec::UX.rotcopy(rrnd(0, PI * 2), vec::UY) * rrnd(0, s * 38) + vec::UY * (0.1 * s);
		real s1 = rrnd(0.25, 1) * s;
		real s2 = rrnd(5, 18) * s;
		
		if (rrnd() < 0.75)
		{			
			color = blendcor(0xFF80FF80, 0xFF80FFFF, rrnd());
			VECLIST e;
			roundedge(e, p, vec::UY, s1 * 3, rrnd(5, 8), rrnd(0, PI)); closeedge(e);
			face(e);
			color = 0xFFFFFFFF;
		}
		else
		{
			color = blendcor(0xFFA0FFA0, 0xFFAAFFFF, rrnd());
			VECLIST e;
			// 底部
			roundedge(e, p, vec::UY, s1 * 3, 3, rrnd(0, PI)); closeedge(e);
			{
				face(e, p + vec::UY * s2);
			}
			color = 0xFFFFFFFF;
			/*if (rrnd() < 0.5)
				triang(p - vec::UX * (s1 * 2), p + vec::UX * (s1 * 2), p + vec::UY * s2);
			else
				triang(p + vec::UX * (s1 * 2), p - vec::UX * (s1 * 2), p + vec::UY * s2);*/
		}
	}
}
void grassArtEX(vec o, vec up, real s)
{
	real s1 = rrnd(0.25, 2) * s;
	real s2 = rrnd(0.25, 0.5) * s;
	VECLIST e;
	// 底部
	roundedge(e, o, vec::UY, s2, 3, rrnd(0, PI)); closeedge(e);
	{// 叶子部分
		for (int i = 0; i < e.size() - 1; i++)
		{
			vec ve = e[i] - e[i + 1];
			ve.norm();
			vec vv = blend(up.cross(ve), up, rrnd(0.1, 0.6)); vv.norm();
			vec pp = (e[i] + e[i + 1]) / 2 + vv * s1;
			triang(e[i], pp, e[i + 1]);
		}
	}
}
// ------------------------------------------------
// 岩石
// ------------------------------------------------
void subline(VECLIST& e, crvec p1, crvec p2, crvec worldpos, int depth = 0)
{
	real len = (p1 - p2).len();
	vec p = blend(p1, p2, 0.5);
	vec dp(rrnd(-1, 1), 0, rrnd(-1, 1)); dp.norm();
	real pn = perlin(worldpos.x + p.x * 2, worldpos.y + p.y * 2, worldpos.z + p.z * 2);
	p += dp * (pn * len / 2);

	if (-1 == insertp(e, p, p1, p2))
	{
		throw;
	}
	if (depth < 1)
	{
		subline(e, p1, p, worldpos, depth + 1);
		subline(e, p, p2, worldpos, depth + 1);
	}
	else
	{
		//line3d(p1, p);
		//line3d(p2, p);		
	}
}

// ------------------------------------------------
void hgtedge(VECLIST& e, vec o, vec uy, real s, crvec worldpos, int coners)
{
	DEF_SCALES(s);
	uy.norm(); vec vx, vy; v2vxvy(uy, vx, vy);
	//roundedge(e, o, vx, vy, s, 4);closeedge(e);
	{
		CIRCLE2(i, s, coners, 0)
			if(i != coners)
				e.PUSH(o + (vx * dpi.x * rrnd(0.75, 1) + vy * dpi.y * rrnd(0.5, 1)));
		END
		closeedge(e);
		
	}
	VECLIST te = e;
	for (int i = 0; i < coners; i++)
	{
		subline(e, te[i], te[i + 1], worldpos);
	}
}
// ------------------------------------------------
void rock(vec o, vec uy, real s, crvec worldpos)
{	
	real rnd = rndbypos(worldpos);
	s *= blend(0.4, 0.6, rnd);
	rnd = rndbypos(worldpos + vec::UX * s);
	int coners = blend(6, 4, rnd);

	color = blendcor(RGB(168, 88, 87), RGB(90, 88, 85), rnd);
	DEF_SCALES(s);
	VECLIST e1;
	hgtedge(e1, o, uy, s1, worldpos, coners);
	vec p = o;
	real ss = s;
	LOOP(i, len, 8)
		VECLIST e2;
		real rnd2 = rndbypos(worldpos + p);
		real rnd3 = rndbypos(worldpos + p + vec::UY * 18.18);
		p += vec(blend(-1, 1, rnd2) * s3, s3, blend(-1, 1, rnd3) * s3);
		hgtedge(e2, p, uy, ss, worldpos, coners);
		face(e1, e2);
		e1 = e2;	
	END
	face(e1, getedgecenter(e1));
}
// ------------------------------------------------
void rockonplane(crvec p1, crvec p2, crvec p3, crvec p4, crvec worldpos, real h0, real prn = 1, real frequency = 0.25, float amplitude = 0.5, int depth = 0)
{
	if (depth < 5)
	{
		real ax = rndbypos(worldpos + p1, 0.4, 0.6);
		real ay = rndbypos(worldpos + p3, 0.4, 0.6);

		vec p12 = blend(p1, p2, ax);
		vec p43 = blend(p4, p3, ax);
		vec p14 = blend(p1, p4, ay);
		vec p23 = blend(p2, p3, ay);
		vec pm = blend(p12, p43, ax);
		
		real pn = ridge(perlin(worldpos.x + pm.x * frequency, worldpos.y + pm.y * frequency, worldpos.z + pm.z * frequency), 1);
		{
			//vec n = gettrinorm(p1, p2, p3);		
			pm.y += (amplitude * prn * pn * h0);
			
			rockonplane(p4, p14, pm, p43, worldpos, h0, pn, frequency * 2.5, amplitude * 0.5, depth + 1);
			rockonplane(p43, pm, p23, p3, worldpos, h0, pn, frequency * 2.5, amplitude * 0.5, depth + 1);
			rockonplane(p1, p12, pm, p14, worldpos, h0, pn, frequency * 2.5, amplitude * 0.5, depth + 1);
			rockonplane(p12, p2, p23, pm, worldpos, h0, pn, frequency * 2.5, amplitude * 0.5, depth + 1);
		}
	}
	else
	{
		plane(p1, p2, p3, p4);
	}
}
// ------------------------------------------------
void rockV(vec o, vec uy, real r0, crvec worldpos)
{
	DEF_SCALES(1);
	real su = 1 / FIMAGESCALE;
	const int len = 15; // 维度
	const int wid = 10; // 经度

	vec lstpt[wid + 1];
	vec llstpt[wid + 1];
	vec lstp;
	real dr = blend(0.2, 0.8, rndbypos(worldpos));
	for (int i = 0; i <= len; i++)
	{
		for (int j = 0; j <= wid; j++)
		{
			vec2 p = vec2(i / real(len), j / real(wid));
			real r = r0 * p.y;
			vec pp = vec((r * (1 - dr)) * cos(PI * 2 * p.x), 0, (r * dr) * sin(PI * 2 * p.x)) + o;
			real ss = r0 * blend(1, 0, p.y, 4);
			{
				real dhgt = RidgedMF(worldpos.x + pp.x * 2.0, worldpos.y + pp.y * 2.0, worldpos.z + pp.z * 2.0, 6, 1);
				pp = pp + vec::UY * (dhgt * ss);
				if (i > 0 && j > 0 && j < wid)
				{	
					//binvnorm = true;
					plane(pp, lstp, llstpt[j], lstpt[j]);			
					//binvnorm = false;
				}
			}
			if (j == 0 || j == wid)
				llstpt[j] = pp;
			else
				llstpt[j] = lstp;
			lstpt[j] = lstp = pp;
		}
	}
}
// ------------------------------------------------
// 荷花
void lotus1(vec o, vec v0, real s)
{
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = 16;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.5);
		vec np = p + (v0 + dp) * (s * 10);
		{
			color = blendcor(0xFF001000, 0xFF008080, ai);
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, s, 8);
			}
			else if (i < len - 2)
			{// 身体
				moveedge(e, (np - p), e1);
			}
			else
			{// 叶子
				moveedge(e, (np - p), e1);
				scaleedge(e1, 1 / E2);
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	face(e, getedgecenter(e) + vec::UY * (s * 1));
	color = 0xFFFFFFFF;
}
// ------------------------------------------------
// 莲蓬
void lotus2(vec o, vec v0, real s)
{
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = 10 *  g_real + 2;//rrnd(8, 16) * g_real;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * (perlin(p.x * 10 + gworldpos.x, p.y * 10, p.z * 10 + gworldpos.z + gtimealpha * 4) * 1.5);
		vec np = p + (v0 + dp) * (s * 10);
		{
			color = blendcor(0xFF001000, 0xFF004040, ai);
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY,  s * 2, 6);
			}
			else if (i < len)
			{// 身体
				moveedge(e, (np - p), e1);
			}
			else
			{// 叶子
				color = blendcor(0xFF001010, 0xFF004040, ai);
				rotedgebynorm(e, (v0 + dp).normcopy());
				moveedge(e, (np - p), e1);				
				scaleedge(e1, 6);
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	face(e, getedgecenter(e) - getedgenorm(e) * (s * 5));
	color = 0xFFFFFFFF;
}
// ------------------------------------------------
void lotusflower(vec o, vec v0, real s,
	int n, 
	int aa,
	int an 
	)
{
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = n;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.5);
		vec np = p + (v0 + dp) * (s * 10);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, 2 * s, aa);
			}
			else if (i < len)
			{// 茎
				color = 0xFF001000;
				moveedge(e, (np - p), e1);
				float sc = blend(0.9, 1, ai, 2.5);
				scaleedge(e1, sc);
				//rotedgebynorm(e1, v0);
				color = 0xFF008000;
			}
			else
			{// 花
				color = 0xFFFFA0FF;
				moveedge(e, (np - p) * 0.02, e1);
				//scaleedge(e1, 1.2);
				for (int ii = 0; ii < aa; ii++)
				{
					VECLIST ee;
					unionedge(e, e1, ii, 2, ee);
					closeedge(ee);
					vec v = getedgenorm(ee);
					real cvg = rrnd(0.025, 0.75);
					for (int jj = 0; jj < an; jj++)
					{
						real ajj = jj / real(an);
						v -= vec::UY * blend(-0.25, cvg, ajj);
						v.norm();
						VECLIST ee1;
						moveedge(ee, v * (s * 5), ee1);
						float sc = blend(1.8, 0.1, ajj, 1.5);
						scaleedge(ee1, sc);
						face(ee, ee1);
						ee = ee1;
					}
				}
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	color = 0xFFFFFFFF;
}
// ------------------------------------------------
void fish(vec o, vec v, real s)
{
	gcommonvertex = 1;
	vec p = o;
	real s1 = s;
	real s2 = 8 * s;
	VECLIST e;
	// 底部
	roundedge(e, p, v, s1, 3, rrnd(0, PI)); closeedge(e);
	{
		binvnorm = true;
		face(e, getedgecenter(e) - v * s2);
		binvnorm = false;
		extrudeedgeex(e, s2, 1);
		face(e, getedgecenter(e) + v * s2);
	}
	gcommonvertex = 0;
}
// ------------------------------------------------
void BLOCK1(const vec& o, const vec& v, float usize = USIZE)
{
	VECLIST e;
	plane(e, o, v, usize, usize);
	closeedge(e);
	binvnorm = 1;
	face(e);
	binvnorm = 0;
	extrudeedgeex(e, usize, 1);
	face(e);
}
void BLOCK2(const vec& o, const vec& vx, const vec& vy, float usize = USIZE)
{
	VECLIST e;
	plane(e, o - vx * usize / 2 - vy * usize / 2, vx, vy, usize, usize);
	closeedge(e);
	binvnorm = 1;
	face(e);
	binvnorm = 0;
	extrudeedgeex(e, usize, 1);
	face(e);
}
// ------------------------------------------------
// 睡莲
// ------------------------------------------------
void Nymphaea_Leaf(vec o, real sz)
{
	color = blendcor(0xFF002010, 0xFF10A010, rrnd());
	VECLIST e1;
	int len = 16;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		real ang = blend(0, PI, ai);
		real r = blend(0, sz, ai, 0.25);
		real deta = (i ==0 || i == len) ? 0 : rrnd(0, sz / 8);
		e1.push_back(o + vec(r * cos(ang), deta, r * sin(ang)));		
	}
	closeedge(e1);
	face(e1);

	VECLIST e2;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		real ang = blend(0, PI, ai);
		real r = blend(0, sz, ai, 0.25);
		real deta = (i == 0 || i == len) ? 0 : rrnd(0, sz / 8);
		e2.push_back(o + vec(r * cos(-ang), deta, r * sin(-ang)));
	}
	vec oo = e2[0];
	invedge(e2);
	closeedge(e2);	
	face(e2, oo);
	color = 0xFFFFFFFF;
}

// ------------------------------------------------
void Nymphaea_Flower(vec o, real s)
{
	vec v0 = vec::UY;
	int n = 4;
	int aa = 12;
	int an = 8;
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = n * g_real;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.5);
		vec np = p + (v0 + dp) * (s * 10);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, 2 * s, aa);
			}
			else if (i < len)
			{// 茎
				color = 0xFF001000;
				moveedge(e, (np - p), e1);
				float sc = blend(0.9, 1, ai, 2.5);
				scaleedge(e1, sc);
				//rotedgebynorm(e1, v0);
				color = 0xFF008000;
			}
			else
			{// 花
				color = blendcor(0xFFB080FF, 0xFF8040FF, rrnd());
				moveedge(e, (np - p) * 0.02, e1);
				//scaleedge(e1, 1.2);
				for (int ii = 0; ii < aa; ii++)
				{
					VECLIST ee;
					unionedge(e, e1, ii, 2, ee);
					closeedge(ee);
					vec v = blend(vec::UY, getedgenorm(ee), rrnd(0.25, 1));
					real cvg = rrnd(0.025, 0.75) * rrnd(0.25, 0.5);
					for (int jj = 0; jj < an; jj++)
					{
						real ajj = jj / real(an);
						v += vec::UY * blend(-0.0, cvg, ajj);
						v.norm();
						VECLIST ee1;
						moveedge(ee, v * (s * 5), ee1);
						float sc = blend(1.8, 0.1, ajj, 1.5);
						scaleedge(ee1, sc);
						face(ee, ee1);
						ee = ee1;
					}
				}
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	face(e, getedgecenter(e));
	color = 0xFFFFFFFF;
}

// ------------------------------------------------------------------------------------------------
// 松柏
void songleaf(vec o, vec v, vec n, real len, real w, int cor0)
{
	color = blendcor(RGB(34, 177, 76), cor0, rrnd(0.25, 1));
	vec vside = v.cross(n); vside.norm();
	triang(o, o + v * len - vside * (w / 2), o + v * len + vside * (w / 2));
	color = 0xFFFFFFFF;
}
void songbai(vec p, vec v, real s)
{
	int cor0 = blendcor(0xFF108040, 0xFF002030, rrnd());
	DEF_SCALES2(s);
	VECLIST e;
	vec uz = vec::UZ;
	v.norm();
	real cf = rrnd(1.25, 2);
	int len = 80;

	color = cor0;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		v.norm();
		vec np = p + (v)* (s);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, v, s * 4, 3);
			}
			else
			{// 身体
				moveedge(e, (np - p), e1);
				rotedgebynorm(e1, v);
				scaleedge(e1, 0.95);
				//real ang = blend(0, PI / 2, ai, cf) * 1;
				//v.rot(ang, uz);
			}
			for (int ii = 0; ii < 18; ii++)
			{// 叶子
				vec tv = vec::UX.rotcopy(rrnd(0, PI * 2), vec::UY);
				vec vv = blend(-v, tv, 0.5); vv.norm();
				vec nv = blend(v, tv, 0.5); nv.norm();
				songleaf(p, vv, nv, s * blend2(4, 18, ai, 2), s * 2.5, cor0);
			}
			if (i != 0 && i < 40)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	color = 0xFFFFFFFF;
}
// ------------------------------------------------------------------------------------------------
// 海带树
void haidaishu(vec p, vec v, real s)
{
	DEF_SCALES2(s);
	VECLIST e;
	vec uz = vec::UZ;
	v.norm();
	real cf = rrnd(1.25, 2);
	int len = 80;

	color = 0xFF222222;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		v.norm();
		vec np = p + (v)* (s);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, v, s * 2, 3);
			}
			else
			{// 身体
				moveedge(e, (np - p), e1);
				rotedgebynorm(e1, v);
				scaleedge(e1, 0.995);
				//real ang = blend(0, PI / 2, ai, cf) * 1;
				//v.rot(ang, uz);
			}
			if (i == len)
			{
				for (int ii = 0; ii < 58; ii++)
				{// 叶子
					vec tv = vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)); tv.norm();
					vec nv = vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)); tv.norm();
					songleaf(p, tv, nv, s * blend2(32, 64, ai), s * 2.5, color);
				}
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	color = 0xFFFFFFFF;
}
// -------------------------------------------------------------
// 冬青
void triangleaf2(vec o, vec v, vec n, real len, real w)
{
	vec vside = v.cross(n); vside.norm();
	triang(o - vside * (w / 2), o + vside * (w / 2), o + v * len);
}
void dongqingLeaf(vec p, vec v, real s)
{
	if(rrnd(0, 1000) < 18)
		color = blendcor(0xFFA040FF, 0xFFDCAAF0, rrnd());
	else
		color = blendcor(0xFF206040, 0xFF00AA80, rrnd());
	for (int ii = 0; ii < 8; ii++)
	{
		real aii = real(ii) / 24;
		real r = rrnd(s * 8, s * 15);
		vec nm = v;
		vec vv = vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)); vv.norm();
		triangleaf2(p + v * r, vv, rrnd() < 0.5 ? nm : -nm, s * 2.0, s * 2.0);
	}
	color = 0xFFFFFFFF;
}

void dongqing(vec p, vec v, real s)
{
	DEF_SCALES2(s);
	VECLIST e;
	vec uz = vec::UZ;
	v.norm();
	real cf = rrnd(1.25, 2);
	int len = 30;

	color = 0xFF222222;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		v.norm();
		vec np = p + (v)* (s);
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, v, s * 2, 3);
			}
			else
			{// 身体
				moveedge(e, (np - p), e1);
				rotedgebynorm(e1, v);
				scaleedge(e1, 0.95);
			}
			if (i == len)
			{
				for (int ii = 0; ii < 250; ii++)
				{// 叶子
					vec tv = vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)); tv.norm();
					dongqingLeaf(p, tv, s * 2);
				}
			}
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	color = 0xFFFFFFFF;
}

// -------------------------------------------------------------
// 棕榈树
// -------------------------------------------------------------
void triangleaf(vec o, vec v, vec n, real len, real w)
{
	vec vside = v.cross(n); vside.norm();
	triang(o - vside * (w / 2), o + vside * (w / 2), o + v * len);
}
void PalmLeaf(vec p, vec v, real s, int len0 = 16)
{
	VECLIST e;
	v.norm();
	for (int i = 0; i < len0; i++)
	{
		vec np = p + v * (s * 18);
		v = v - vec::UY * 0.025;

		if (i == 0)
			roundedge(e, p, v, s, 3);
		else
		{// 茎	
			VECLIST e1;
			vec gv = (np - p);
			if (i < len0)
			{
				scaleedge(e, 0.8, e1);
				moveedge(e1, gv);
				face(e, e1);
				e = e1;
			}
		}
		if (i == len0 - 1)
		{// 叶子	
			color = blendcor(0xFF004000, 0xFF00AA00, rrnd());
			for (int ii = 0; ii < 18; ii++)
			{
				real aii = real(ii) / 18;
				vec nm = v.cross(vec::UY); nm.norm();
				vec vv = v.rotcopy(blend(-PI * 0.8, PI * 0.8, aii), nm);
				triangleaf(p, vv, rrnd() < 0.5 ? nm : -nm, s * blend2(18, 64, aii), s * 4.0);
			}
			color = 0xFFFFFFFF;
		}
		p = np;
	}
}
// -------------------------------------------------------------
void PalmTree(vec o, vec v0, real s, int len0 = 24, int depth = 0)
{
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = len0;
	real dpdeta = rrnd(0, 12.862);
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * ((-0.5 + perlin(p.x * 10, p.y * 10, p.z * 10 + dpdeta)) * 0.25) + vec::UZ * ((-0.5 + perlin(p.x * 10 + dpdeta, p.y * 10, p.z * 10)) * 0.25);
		vec np = p + (v0 + dp - vec::UY * 0.1) * (s * 10);
		{
			if (i == 0)
			{// 底部
				roundedge(e, p, vec::UY, s / E2, 6);
			}
			else
			{// 身体
				real dlen = (np - p).len();
				vec gv = (np - p);
				{// 茎	
					scaleedge(e, 0.995f);
					VECLIST e1;
					moveedge(e, gv, e1);

					VECLIST e2;
					scaleedge(e1, 1.2, e2);
					face(e, e2);
					e = e1;
				}
				if (i > 0.75 * len)
				{// 分支
					int num = rrnd(5, 8);
					for (int ii = 0; ii < num; ii++)
					{
						vec vv = (gv * rrnd(0.5, 1) + vec(rrnd(-1, 1), rrnd(0, 1), rrnd(-1, 1)).normcopy());
						vv.rot(rrnd(-PI, PI), gv.normcopy());
						PalmLeaf(p, vv, s * 0.5, len * rrnd(0.2, 0.5));
					}
				}
			}
		}
		p = np;
	}
}
// -------------------------------------------------------------
// 喷泉
void penquan(crvec o, real s)
{
	VECLIST e;
	roundedge(e, o, vec::UY, s, 8, PI / 8);
	color = blendcor(1, 0xFF5A4E6A, 0.5);
	extrudeedgeex(e, s * E, E);	
	color = blendcor(1, 0xFF5A4E6A, 0.75);
	face(e, getedgecenter(e));
	gtargets.push_back(getedgecenter(e));
	color = 0xFFFFFFFF;	
}
// ------------------------------------------------
// 盆花 
// ------------------------------------------------
void huabantriangle(vec o, vec v, vec n, real len, real w)
{	
	vec vside = v.cross(n); vside.norm();
	triang(o, o + v * len - vside * (w / 2), o + v * len + vside * (w / 2));	
}
void huabanleave(vec o, vec v, vec n, real len, real w)
{
	len /= 2;
	vec vside = v.cross(n); vside.norm();
	triang(o, o + v * len - vside * (w / 2), o + v * len + vside * (w / 2));
	triang(o + v * len - vside * (w / 2), o + v * len + vside * (w / 2), o + v * (len * 2));
}
void flowerunit(vec o, vec face, real sz, int n)
{
	vec dx, dy;
	v2vxvy(face, dx, dy);
	vec fp = o+vec::UY * (USIZE * rrnd(1, 1.75));
	vec lp = o + vec::UY * (USIZE * 0.1);

	//color = blendcor(RGB(134, 0, 176), 0xFF0000FF, rrnd(0.25, 1));
	color = blendcor(g_uint, RGB(134, 0, 176), rrnd(0, 0.5));
	for (int i = 0; i < n; i++)
	{// flower
		real ang = PI * 2 * i / real(n);
		vec v = dx * cos(ang) + dy * sin(ang);
		huabantriangle(fp, v, face, sz / 1.5, sz / 3);			
	}
	for (int ii = 0; ii < 3; ii++)
	{// leave		
		color = blendcor(RGB(50, 50, 0), 0xFF00FF00, rrnd(0, 1));
		huabanleave(lp, blend(vec::UY, vec::UZ.rotcopy(rrnd(-PI, PI)), rrnd()), vec::UY, sz * 1.8, sz);
		color = 0xFFFFFFFF;
	}
	/*if (rrnd() < 0.01)
	{
		Asterids(o, vec::UY, sz);
	}*/
}
void flowerplane(vec p1, vec p2, vec p3, vec p4)
{
	int wid = MIN(2000, (p2 - p1).len() / USIZE * 1.5);
	int hgt = MIN(2000, (p4 - p1).len() / USIZE * 1.5);

	for (int i = 0; i < wid; i ++)
	for (int j = 0; j < hgt; j ++)
	{
		real posx = rrnd();
		vec p12 = blend(p1, p2, posx);
		vec p34 = blend(p4, p3, posx);
		vec p = blend(p12, p34, rrnd(0, 1));
		flowerunit(p, vec::UZ.rotcopy(rrnd(-PI, PI)), rrnd(USIZE / 4, USIZE), 3);
	}
}
void flowertriangle(vec p1, vec p2, vec p3)
{
	int wid = (p2 - p1).len() / USIZE, hgt = (p3 - p1).len() / USIZE;
	for (int i = 0; i < wid; i++)
		for (int j = 0; j < hgt; j++)
		{
			vec p12 = blend(p1, p2, rrnd());			
			vec p = blend(p12, p3, rrnd());
			flowerunit(p, vec::UZ.rotcopy(rrnd(-PI, PI)), rrnd(USIZE / 4, USIZE), 3);
		}
}
// -------------------------------------------------------------
// 梅
// -------------------------------------------------------------
void dongqingLeaf2(vec p, vec v, real s)
{
	if (rrnd(0, 1000) < 18)
		color = blendcor(0xFF2080FF, 0xFFDCAAF0, rrnd());
	else
		color = blendcor(0xFFFFFFFF, 0xFFAAFFDA, rrnd());
	real dpdeta = rrnd(0, 12.862);
	real rm = rrnd(s * 28, s * 45);
	for (int ii = 0; ii < 15; ii++)
	{
		real aii = real(ii) / 24;
		real r = rrnd(s * 5, rm);
		vec nm = v;
		vec vv = vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)); vv.norm();
		vec pp = p + v * r;
		vec dp = vec::UX * ((-0.5 + perlin(p.x * 10, p.y * 10, p.z * 10 + dpdeta)) * 0.75) + vec::UZ * ((-0.5 + perlin(p.x * 10 + dpdeta, p.y * 10, p.z * 10)) * 0.75);

		triangleaf2(pp + dp * (s * 10), vv, rrnd() < 0.5 ? nm : -nm, s * 3.0, s * 2.0);
	}
	color = 0xFFFFFFFF;
}
// -------------------------------------------------------------
void PlumTree(vec o, vec v0, real s, int len0 = 18, int depth = 0)
{
	color = blendcor(0xFF102000, 0xFF104010, rrnd());
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = len0;
	real dpdeta = rrnd(0, 12.862);
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * ((-0.5 + perlin(p.x * 10, p.y * 10, p.z * 10 + dpdeta)) * 0.25) + vec::UZ * ((-0.5 + perlin(p.x * 10 + dpdeta, p.y * 10, p.z * 10)) * 0.25);
		vec np = p + (v0 + dp + vec::UY * 0.2) * (s * 10);
		{
			if (i == 0)
			{// 底部
				roundedge(e, p, vec::UY, s / E4, 6);
			}
			else
			{// 身体
				real dlen = (np - p).len();
				vec gv = (np - p);
				{// 茎	
					scaleedge(e, 0.95f);
					VECLIST e1;
					moveedge(e, gv, e1);					
					face(e, e1);
					e = e1;
				}			
				if (i == len)
				{
					for (int ii = 0; ii < 300; ii++)
					{// 叶子
						vec tv = vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)); tv.norm();
						dongqingLeaf2(p, tv, s * 3);
					}
					color = 0xFFFFFFFF;
				}
			}
		}
		p = np;
	}
}
// -------------------------------------------------------------
// 蝴蝶
// -------------------------------------------------------------
void butterfly_triangle(vec o, vec v, vec n, real len, real w, int sex)
{
	vec vside = v.cross(n); vside.norm();
	color = sex == 0 ? 0xFFFF0000 : 0xFF0000FF;
	triang(o, o + v * len - vside * (w / 2), o + v * len + vside * (w / 2));
	// border
	color = 1;
	triang(o + v * len - vside * (w / 2), o + v * (len * 1.2) - vside * (w / 2), o + v * len + vside * (w / 2));
}
void butterfly(vec o, vec v, real sz, real t, int sex)
{
	sz *= 2;	
	real st = 0.5 + 0.5 * sin(t * PI * 4);
	vec left0 = vec::UY.cross(v); left0.norm();
	vec up0 = v.cross(left0);	
	{
		vec left = blend(left0, up0, st);
		vec up = v.cross(left);
		vec lwing1 = blend(left, v, 0.5);
		butterfly_triangle(o, lwing1, up, sz, sz / 2, sex);
		vec lwing2 = blend(left, -v, 0.5);
		butterfly_triangle(o, lwing2, up, sz / 2, sz / 4, sex);
	}
	{
		vec right = blend(-left0, up0, st);
		vec up = v.cross(right);		
		vec rwing1 = blend(right, v, 0.5);
		butterfly_triangle(o, rwing1, up, sz, sz / 2, sex);
		vec rwing2 = blend(right, -v, 0.5);
		butterfly_triangle(o, rwing2, up, sz / 2, sz / 4, sex);
	}
	color = 0xFFFFFFFF;
}
// ################################################################################################
// 直接绘制到画布上
// ################################################################################################
void draw()
{
	DEF_SCALES(1); renderstate = 1;	CAM_POS = vec::CENTER;

	//rockV(vec::CENTER, vec::UY, 0.25, vec::CENTER);
	Nymphaea_Leaf(vec::CENTER, 0.01);
	//gongmentop(vec::CENTER - vec::UX * 0.1, vec::CENTER - vec::UX * 0.2, vec::UY, 0.25);
	return;
}
// ------------------------------------------------
void init()
{	
	light.norm();
}
// ------------------------------------------------
void bk()
{
	int ox = 1.0 * IMAGESCALE;
	int oy = -1.2 * IMAGESCALE;
	for (int j = 0; j < IMAGESCALE; j++)
	for (int i = 0; i < IMAGESCALE; i++)
	{
		drawmap[i][j] = 1;	
	}	
	for (int j = 0; j < 256; j++)
		point(rrnd(0, 1), rrnd(0, 1), rrnd(1, 16), blendcor(0xFF101010, 0xFFFFFFFF, rrnd()));
	
}
// -------------------------------------------------
int form()
{
	fheightmap = fheightmap0;
	perlinmap(fheightmap, IMAGESCALE, &fmin1, &fmax1, 6);
	
	init();
	bk();
	draw();
	
	PRINT("")
	return 0;
}
// -------------------------------------------------
// 绘制场景 并保存效果图
// -------------------------------------------------
unsigned char buf[IMAGESCALE * IMAGESCALE * 3];
void drawScene(const char* path)
{
	char imgname[512];
	srand(18858146);
	for (int i = 0; i < 2; i++)
	{
		// clear
		memset(drawmap0, 0, sizeof(drawmap0));
		memset(drawmap1, 0, sizeof(drawmap1));
		memset(depthmap0, 0, sizeof(depthmap0));
		memset(maskmap, 0, sizeof(maskmap));

		printf("generating image ... \n");

		for (int ii = 0; ii < 512; ii++)
		for (int jj = 0; jj < 512; jj++)
		{
			rndmap[ii][jj] = rrnd(0, 1);
		}
		form();
		printf("done!\n");

		printf("saving ... ");
		sprintf_s(imgname, "%s/img%d.bmp", path, i);
		for (int ii = 0; ii < IMAGESCALE; ii++)
			for (int jj = 0; jj < IMAGESCALE; jj++)
			{
				buf[3 * (jj * IMAGESCALE + ii) + 0] = GetBValue(drawmap[ii][jj]);
				buf[3 * (jj * IMAGESCALE + ii) + 1] = GetGValue(drawmap[ii][jj]);
				buf[3 * (jj * IMAGESCALE + ii) + 2] = GetRValue(drawmap[ii][jj]);
			}
		bmp_write(buf, IMAGESCALE, IMAGESCALE, imgname);

		printf("\ndone!\n");
	}
}
// ################################################################################################
// 动态绘制到场景里
// ################################################################################################
// 3d造型艺术
// ------------------------------------------------------------------------------------------------
extern "C"
{
	bool EXPORT_API createmesh(int type, int dat, vec worldpos)
	{
		gworldpos = worldpos;
		if (type == 0)
		{// 主体
			START_MESH
			VECLIST e;
			plane(e, vec::CENTER, vec::UY, USIZE * 12, USIZE * 12);
			closeedge(e);
			face(e, 1);
			// 直接生成第一个房间
			real d = USIZE * 20;
			extrudeedgeex(e, d, 1);
			face(e);
			END_MESH
			return true;
		}
		else if (type == 1)
		{// 合成器
			START_MESH		
			magicbox(vec::CENTER, dat, .1);
			END_MESH
			return true;
		}
		else if (type == 2)
		{// 小车
			START_MESH
			rover(.1);
			END_MESH
			return true;
		}
		else if (type == 3)
		{// 路灯
			START_MESH
			roadlight(vec::ZERO, vec::UY, 0.0025);
			END_MESH
			return true;
		}
		else if (type == 4)
		{// 小块
			START_MESH
			BLOCK1(vec::ZERO, vec::UY);
			END_MESH
			return true;		
		}
		else if (type == 5)
		{// 平台
			START_MESH
			VECLIST e;			
			plane(e, vec::CENTER, vec::UX, vec::UZ, USIZE * 1, USIZE * 1);
			closeedge(e);
			face(e, true);
			extrudeedgeex(e, USIZE, 1);
			face(e);		
			END_MESH
			return true;
		}
		else if (type == 6)
		{// 岩石
			START_MESH
			gcommonvertex = 1;
			rock(vec::ZERO, vec::UY, USIZE * 25, worldpos);
			gcommonvertex = 0;
			color = 0xFFFFFFF;
			END_MESH
			return true;
		}
		else if (type == 7)
		{// 庄稼
			START_MESH
			BLOCK1(vec::ZERO, vec::UY, USIZE);
			END_MESH
			return true;
		}
		else if (type == 8)
		{// 石柱
			START_MESH
			zhu(vec::ZERO, vec::UY * (USIZE * 8), (USIZE * 10) / 10);
			END_MESH
			return true;
		}
		else if (type == 9)
		{// 拱门
			START_MESH
			real h = fixedSZ((USIZE * 8) / E1);
			gongmen(vec::ZERO - vec::UX * (USIZE * 4), vec::ZERO + vec::UX * (USIZE * 4), vec::UY, h);
			END_MESH
			return true;
		}
		else if (type == 10)
		{// 三角面
			START_MESH
			real sz = USIZE * 2;
			vec p = vec::ZERO - vec::UX * sz;
			color = g_uint;
			triang(vec::ZERO - vec::UY * sz, vec::ZERO + vec::UY * sz, p);
			END_MESH
			return true;
		}
		else if (type == 11)
		{// 诸葛菜
			START_MESH
			real sz = USIZE * 0.25;
			grassArt(vec::ZERO + vec(rrnd(-sz, sz), 0, 0).rotcopy(rrnd(0, PI), vec::UY), vec::UY, sz);
			END_MESH
			return true;
		}		
		else if (type == 13)
		{// 莲
			START_MESH
			gcommonvertex = 1;
			//lotus1(vec::ZERO, vec::UY, USIZE * 0.1);
			int rd = g_int;//rand() % 18;
			//if (rd == 0)
			//	lotusflower(vec::ZERO, vec::UY, USIZE * 0.1, 16, 8, 16);// 荷花 
			if (rd == 2 || rd == 3)
				Nymphaea_Flower(vec::ZERO + vec::UY * (USIZE * 1), USIZE * rrnd(0.12, 0.2));// 睡莲花朵
			else if (rd < 8)
				lotus2(vec::ZERO + vec::UY * (USIZE * 1), vec::UY, USIZE * rrnd(0.05, 0.15));// 莲蓬
			else
				Nymphaea_Leaf(vec::ZERO + vec::UY * (USIZE * 1), USIZE * rrnd(8, 12));// 睡莲叶子
			gcommonvertex = 0;
			END_MESH
			return true;
		}
		else if (type == 14)
		{// fish
			START_MESH
			gcommonvertex = 1;
			fish(vec::ZERO, vec::UZ, USIZE * 0.1);
			gcommonvertex = 0;
			END_MESH
			return true;
		}
		
		else if (type == 16)
		{// 喷泉
			gcommonvertex = 1;
			START_MESH
			penquan(vec::ZERO, USIZE * 2);
			END_MESH
			gcommonvertex = 0;			
			return true;
		}
		else if (type == 17)
		{// 莲花的花苞
			gcommonvertex = 1;
			START_MESH			
			color = 0xFF40FF80;
			VECLIST e;			
			roundedge(e, vec::CENTER, vec::UY, USIZE * 2, 5);
			closeedge(e);			
			extrudeedgeex(e, USIZE * 4, 2.5);
			color = g_uint;
			face(e, getedgecenter(e) + vec::UY * (USIZE * 8));	
			color = 0xFFFFFFFF;			
			END_MESH
			gcommonvertex = 0;
			return true;
		}
		else if (type == 18)
		{// 幻化
			START_MESH
			magicCubeEx(vec::CENTER, dat, .1);
			END_MESH
			return true;
		}
		else if (type == 19)
		{// 刷子
			gcommonvertex = 0;
			START_MESH
			color = g_uint;
			VECLIST e;
			roundedge(e, vec::CENTER, vec::UY, USIZE * 0.5, 3);
			closeedge(e);
			extrudeedgeex(e, USIZE * 3, 5);
			gcommonvertex = 1;			 
			face(e, getedgecenter(e) + vec::UY * (USIZE * 4));
			color = 0xFFFFFFFF;
			END_MESH
			gcommonvertex = 0;
			return true;
		}
		else if (type == 20)
		{// 蝴蝶
			START_MESH
			gcommonvertex = 1;
			butterfly(vec::ZERO, vec::UZ, USIZE * 1, g_real2, dat);
			gcommonvertex = 0;
			END_MESH
			return true;
		}	
		else if (type == 21)
		{// 立体雕刻本体
			START_MESH
			VECLIST e;
			roundedge(e, vec::CENTER + vec::UY * USIZE, -vec::UY, USIZE, 3);
			face(e, true);
			face(e, vec::CENTER);
			END_MESH
			return true;
		}
		else if (type == 22)
		{// 轨道点
			START_MESH
			VECLIST e;
			roundedge(e, vec::CENTER, vec::UY, USIZE * 4, 6);
			//extrudeedgeex(e, USIZE, 1);
			//extrudeedgeex(e, USIZE, 0.5);
			face(e, getedgecenter(e) + vec::UY * (USIZE * 3));
			END_MESH
				return true;
		}
		return false;
	}
	// -------------------------------------------------
	bool EXPORT_API createmeshex(int type, vec o, vec up, int dat)
	{
		if (type == 1)
		{// 诸葛菜
			START_MESH
			real sz = USIZE * 1;
			grassArt(o, up, sz);
			END_MESH
			return true;
		}
		return false;
	}
}
// -------------------------------------------------
// 建筑
// 面上3d互动造型艺术
// -------------------------------------------------
real getAngle(real alpha)
{
	real ang = 0;
	if (alpha > 0.75)
		ang = PI / 3;
	else if (alpha > 0.5)
		ang = PI / 4;
	else if (alpha > 0.25)
		ang = PI / 6;

	return ang;
}
int editface(int type, VECLIST& e, const vec& startpos, const vec& updir, real hitdis)
{
	ghitdis = hitdis;
	if (type == 1)
	{// 窗户
		window(e[0], e[1], e[2], e[3]);
		return true;
	}
	else if (type == 2)
	{// 台阶
		vec n = getedgenorm(e);
		if (n.y > 0.9)
		{
			vec forward = (e[3] - e[2]).cross(vec::UY); forward.norm();
			for (int i = 0; i < 4; i++)
			{
				vec p1 = blend(e[2], e[3], 0.4);
				vec p2 = blend(e[2], e[3], 0.6);
				real wid = (p2 - p1).len();
				p1 += forward * (wid / 5);
				p2 += forward * (wid / 5);
				vec g1 = p1 + forward * (wid * 2); g1.y = 0;
				vec g2 = p2 + forward * (wid * 2); g2.y = 0;
				int num = (g1 - p1).len() / 0.025;
				color = 1;
				staris(p1, p2, g1, g2, num + 8);
				color = 0xFFFFFFFF;
			}
		}
		return true;
	}
	else if (type == 3)
	{// 分割面
		real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		
		bool ret = wall1(e[0], e[1], e[2], e[3], a01, a03, false, 0);
		color = 0xFFFFFFFF;
		return ret;
	}
	else if (type == 4)
	{// 方门
		if (abs(e[0].p.y - e[1].p.y) < abs(e[2].p.y - e[1].p.y))
			if (e[0].p.y < e[2].p.y)
				rectdoor(e[0], e[1], e[2], e[3]);
			else
				rectdoor(e[2], e[3], e[0], e[1]);
		else
			if (e[1].p.y < e[3].p.y)
				rectdoor(e[1], e[2], e[3], e[0]);
			else
				rectdoor(e[3], e[0], e[1], e[2]);
		return true;
	}
	else if (type == 5)
	{// 凸出
		real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		if (a01 > 0.33 && a01 < 0.66 && a03 > 0.33 && a03 < 0.66)
		{// 中凸			
			int ret = wall3(e[0], e[1], e[2], e[3]);
			color = 0xFFFFFFFF;
			return ret;
		}
		else
		{// 角凸			
			int ret = wall2(e[0], e[1], e[2], e[3], a01, a03);
			color = 0xFFFFFFFF;
			return ret;
		}
	}
	else if (type == 6)
	{// 日形窗
		if (abs(e[0].p.y - e[1].p.y) < abs(e[2].p.y - e[1].p.y))
			riwindow(e[0], e[1], e[2], e[3]);
		else
			riwindow(e[1], e[2], e[3], e[0]);
		return true;
	}
	else if (type == 7)
	{// 圆门	
		if (abs(e[0].p.y - e[1].p.y) < abs(e[2].p.y - e[1].p.y))
		{
			if (e[0].p.y < e[2].p.y)
				rounddoor(e[0], e[1], e[2], e[3]);
			else
				rounddoor(e[2], e[3], e[0], e[1]);
		}
		else
		{
			if (e[1].p.y < e[3].p.y)
				rounddoor(e[1], e[2], e[3], e[0]);
			else
				rounddoor(e[3], e[0], e[1], e[2]);
		}
		return true;
	}
	else if (type == 8)
	{// 尖顶
		real len01 = (e[0] - e[1]).len();
		real len02 = (e[0] - e[3]).len();
		real len = MAX(len01, len02);
		vec oo = getedgecenter(e);
		closeedge(e);
		if(rrnd() < 0.8)
			color = blendcor(RGB(6, 50, 159), RGB(6, 124, 170), rrnd());
		else
			color = blendcor(RGB(153, 107, 144), 0xFF5000A0, rrnd());
		if (abs(len01 - len02) < len / 2)
		{
			scaleedge(e, 1 + E6);
			binvnorm = true;
			face(e, oo);
			binvnorm = false;
			if (len < USIZE * 15)
				face(e, oo + getedgenorm(e) * (len * E));
			else
				face(e, oo + getedgenorm(e) * (len * E2));
		}
		else
		{
			real lenm = MIN(len01, len02);
			real deta = 0;
			if (lenm < USIZE * 15)
				deta = (lenm * E);
			else
				deta = (lenm * E2);
			if (len01 > len02)
			{
				vec dv = e[1] - e[0]; dv.norm();
				vec pp = oo + getedgenorm(e) * deta;
				vec pp1 = pp - dv * (len01 / 4);
				vec pp2 = pp + dv * (len01 / 4);
				{
					VECLIST ee; ee.push_back(e[0]); ee.push_back(e[1]); ee.push_back(pp2); ee.push_back(pp1); closeedge(ee); face(ee);
				}
				{
					VECLIST ee; ee.push_back(e[2]); ee.push_back(e[3]); ee.push_back(pp1); ee.push_back(pp2); closeedge(ee); face(ee);
				}
				triang(e[0], pp1, e[3], color);
				triang(e[2], pp2, e[1], color);
			}
			else
			{
				vec dv = e[3] - e[0]; dv.norm();
				vec pp = oo + getedgenorm(e) * deta;
				vec pp1 = pp - dv * (len02 / 4);
				vec pp2 = pp + dv * (len02 / 4);
				{
					VECLIST ee; ee.push_back(e[1]); ee.push_back(e[2]); ee.push_back(pp2); ee.push_back(pp1); closeedge(ee); face(ee);
				}
				{
					VECLIST ee; ee.push_back(e[3]); ee.push_back(e[0]); ee.push_back(pp1); ee.push_back(pp2); closeedge(ee); face(ee);
				}
				triang(e[1], pp1, e[0], color);
				triang(e[3], pp2, e[2], color);
			}
		}
		color = 0xFFFFFFFF;
		return 2;
	}
	else if (type == 9)
	{// 平面叠加
		int lum = rrnd(200, 255);
		color = RGB(lum, lum, 255);
		closeedge(e);
		real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		if (a01 > 0.33 && a01 < 0.66 && a03 > 0.33 && a03 < 0.66)
		{
			extrudeedgeex(e, hitdis, 1);
		}
		else
		{
			extrudeedgeex(e, USIZE, 1);
		}
		face(e);
		color = 0xFFFFFFFF;

		return true;
	}
	else if (type == 10)
	{// 斜面
		real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		vec n = getedgenorm(e);
		if (//MIN((e[0] - e[1]).len(), (e[1] - e[2]).len()) > USIZE &&
			a01 > 0.2 && a01 < 0.8 && a03 > 0.2 && a03 < 0.8)
		{// 中部
			if(a01 > 0.4 && a01 < 0.6 && a03 > 0.4 && a03 < 0.6)
			{
				int lum = rrnd(200, 255);
				color = RGB(lum, lum, 255);
				closeedge(e);
				extrudeedgeex(e, USIZE, 1);
				face(e);
				color = 0xFFFFFFFF;
			}
			else
			{
				real len = MAX((e[0] - e[1]).len(), (e[1] - e[2]).len()) / 2;
				real deta = MIN(hitdis, len / 2);
				if (abs(a01 - 0.5) < abs(a03 - 0.5))
				{
					real d = deta;
					vec p1 = (e[0] + e[3]) / 2 + n * d;
					vec p2 = (e[1] + e[2]) / 2 + n * d;
					plane(e[0], e[1], p2, p1);
					plane(p1, p2, e[2], e[3]);
					triang(e[1], e[2], p2);
					triang(e[0], p1, e[3]);
				}
				else
				{
					real d = deta;
					vec p1 = (e[0] + e[1]) / 2 + n * d;
					vec p2 = (e[2] + e[3]) / 2 + n * d;
					plane(e[1], e[2], p2, p1);
					plane(e[0], p1, p2, e[3]);
					triang(e[0], e[1], p1);
					triang(e[3], p2, e[2]);
				}
			}			
		}
		else
		{// 边缘
			if (abs(a01 - 0.5) < abs(a03 - 0.5))
			{
				real deta = MIN(hitdis, (e[2] - e[1]).len());
				real d = deta;
				vec p1 = (e[0] + e[3]) / 2 + n * d;
				vec p2 = (e[1] + e[2]) / 2 + n * d;
				if (a03 < 0.5)
				{
					p1 = e[0] + n * d;
					p2 = e[1] + n * d;
				}
				else
				{
					p1 = e[3] + n * d;
					p2 = e[2] + n * d;
				}
				plane(e[0], e[1], p2, p1);
				plane(p1, p2, e[2], e[3]);
				triang(e[1], e[2], p2);
				triang(e[0], p1, e[3]);
			}
			else
			{
				real deta = MIN(hitdis, (e[0] - e[1]).len());
				real d = deta;
				vec p1 = (e[0] + e[1]) / 2 + n * d;
				vec p2 = (e[2] + e[3]) / 2 + n * d;
				if (a01 < 0.5)
				{
					p1 = e[0] + n * d;
					p2 = e[3] + n * d;
				}
				else
				{
					p1 = e[1] + n * d;
					p2 = e[2] + n * d;
				}
				plane(e[1], e[2], p2, p1);
				plane(e[0], p1, p2, e[3]);
				triang(e[0], e[1], p1);
				triang(e[3], p2, e[2]);
			}
		}
		return true;
	}
	else if (type == 11)
	{// 层叠
		real len01 = (e[0] - e[1]).len();
		real len02 = (e[0] - e[3]).len();
		real len = MAX(len01, len02);

		int lum = rrnd(200, 255);
		color = RGB(lum, lum, 255);
		closeedge(e);

		real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		if (a01 > 0.33 && a01 < 0.66 && a03 > 0.33 && a03 < 0.66)
		{
			extrudeedgeex(e, len, 1);
		}
		else
		{
			extrudeedgeex(e, USIZE, 1);
		}
		face(e);
		color = 0xFFFFFFFF;
		return true;
	}
	else if (type == 12)
	{// 层叠(类延伸预览用)
		int lum = rrnd(200, 255);
		color = RGB(lum, lum, 255);
		closeedge(e);
		real d = hitdis;
		face(e, true);

		extrudeedgeex(e, hitdis, 1);
		face(e);
		color = 0xFFFFFFFF;
		return true;
	}
	else if (type == 13)
	{// 栅栏
		color = RGB(1, 1, 1);
		real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		if (abs(a01 - 0.5) < abs(a03 - 0.5))
		{
			if (a03 < 0.5)
				column(e[0], e[1], getedgenorm(e));
			else
				column(e[2], e[3], getedgenorm(e));
		}
		else
		{
			if (a01 < 0.5)
				column(e[3], e[0], getedgenorm(e));
			else
				column(e[2], e[1], getedgenorm(e));

		}
		color = 0xFFFFFFFF;
		return 2;
	}
	else if (type == 14 || type == 15 || type == 16)
	{// 智能台阶
		{// 移动基面			
			vec dv = getedgenorm(e);
			if (dv.dot(updir) > 0.25) // 只有水平方向
				return 0;

			vec dx = updir.cross(dv).normcopy();
			real ang = 0;
			{// 判断旋转角度				
				if (abs(e[0].p.dot(updir) - e[1].p.dot(updir)) < abs(e[1].p.dot(updir) - e[2].p.dot(updir)))
				{
					real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
					if ((e[1] - e[0]).dot(dx) < 0)
					{
						a01 = 1 - a01;
					}
					ang = PI / 8 * (blend(0, 2, a01) - 1);
					ang = int(ang / (PI / 32)) * (PI / 32);// 对齐					
				}
				else
				{
					real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
					if ((e[3] - e[0]).dot(dx) < 0)
					{
						a03 = 1 - a03;
					}
					ang = PI / 8 * (blend(0, 2, a03) - 1);
					ang = int(ang / (PI / 32)) * (PI / 32);// 对齐					
				}
			}
			rotedge(e, ang, updir);
		}
		if (type == 16)
		{// 上升
			closeedge(e);
			real d0 = USIZE;//MAX(2 * USIZE, MIN((e[0] - e[1]).len(), (e[2] - e[1]).len()) / 2);
			moveedge(e, updir * d0);
			face(e, 1);
			int lum = rrnd(200, 255); color = RGB(lum, lum, 255);
			real d = MAX(USIZE, MAX((e[0] - e[1]).len(), (e[2] - e[1]).len()) / 16);
			extrudeedgeex(e, d, 1);
			face(e);
			color = 0xFFFFFFFF;
		}
		else if (type == 15)
		{// 平行
			closeedge(e);
			face(e, 1);
			int lum = rrnd(200, 255); color = RGB(lum, lum, 255);
			real d = MAX(4 * USIZE, MAX((e[0] - e[1]).len(), (e[2] - e[1]).len()) * E2);
			moveedge(e, vec::UY * (rrnd(-0.01, 0.01) * USIZE));
			extrudeedgeex(e, d, 1);
			face(e);
			color = 0xFFFFFFFF;
		}
		else if (type == 14)
		{// 下降
			closeedge(e);
			real d0 = USIZE;//MAX(USIZE, MIN((e[0] - e[1]).len(), (e[2] - e[1]).len()));
			moveedge(e, -updir * d0);
			face(e, 1);
			int lum = rrnd(200, 255); color = RGB(lum, lum, 255);
			real d = MAX(USIZE, MAX((e[0] - e[1]).len(), (e[2] - e[1]).len()) / 16);
			extrudeedgeex(e, d, 1);
			face(e);
			color = 0xFFFFFFFF;
		}
		return 2;
	}
	else if (type == 17)
	{// 平顶
		real len = MAX((e[0] - e[1]).len(), (e[1] - e[2]).len());
		if (rrnd() < 0.5)
			color = blendcor(0xFF600030, 0xFFA00000, rrnd());
		else
			color = blendcor(0xFF800040, 0xFFA00040, rrnd());
		closeedge(e);
		VECLIST e2;
		moveedge(e, vec::UY * (1 * USIZE), e2);
		face(e, e2);
		face(e2);
		color = 0xFFFFFFFF;
		return 2;
	}
	else if (type == 18)
	{// 圆顶
		real len = MIN((e[0] - e[1]).len(), (e[1] - e[2]).len());
		color = blendcor(0xFFFFFFA0, 0xFFA00080, rrnd());

		yuanding(getedgecenter(e), getedgenorm(e), len);

		color = 0xFFFFFFFF;
		return 2;
	}
	else if (type == 19)
	{// 围墙
		{// 移动基面			
			vec dv = getedgenorm(e);
			if (dv.dot(updir) > 0.25) // 只有水平方向
				return 0;

			vec dx = updir.cross(dv).normcopy();
			vec updir0;
			real ang = 0;
			
			VECLIST e0 = e; closeedge(e0);

			// 判断旋转角度				
			if (abs(e[0].p.dot(updir) - e[1].p.dot(updir)) < abs(e[1].p.dot(updir) - e[2].p.dot(updir)))
			{
				real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();	
				if ((e[1] - e[0]).dot(dx) < 0)
				{					
					if (a01 < 0.5)
					{	
						ang = getAngle(1-a01 / 0.5);						
						if (ang != 0) rotedgebycenter(e, ang, updir, e[0]);
					}
					else
					{		
						ang = getAngle((a01 - 0.5) / 0.5);
						if (ang != 0) rotedgebycenter(e, -ang, updir, e[1]);
					}
				}
				else
				{
					if (a01 < 0.5)
					{
						ang = getAngle(1-a01 / 0.5);	
						if (ang != 0) rotedgebycenter(e, -ang, updir, e[0]);
					}
					else
					{
						ang = getAngle((a01 - 0.5) / 0.5);
						if (ang != 0) rotedgebycenter(e, ang, updir, e[1]);
					}					
				}					
				if (ang == 0)	return false;					
					//moveedge(e, dv * (8 * USIZE));

				closeedge(e);
				face(e0, e);
				face(e);
			}
			else
			{
				real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
				VECLIST e0 = e; closeedge(e0);
				if ((e[3] - e[0]).dot(dx) < 0)
				{
					if (a03 < 0.5)
					{
						ang = getAngle(1 - a03 / 0.5);
						if (ang != 0) rotedgebycenter(e, ang, updir, e[0]);
					}				
					else
					{
						ang = getAngle((a03 - 0.5) / 0.5);
						if (ang != 0) rotedgebycenter(e, -ang, updir, e[3]);
					}
				}
				else
				{
					if (a03 < 0.5)
					{
						ang = getAngle(1 - a03 / 0.5);
						if (ang != 0) rotedgebycenter(e, -ang, updir, e[0]);
					}
					else
					{
						ang = getAngle((a03 - 0.5) / 0.5);
						if (ang != 0) rotedgebycenter(e, ang, updir, e[3]);
					}
				}
				if (ang == 0)	return false;
					//moveedge(e, dv * (4 * USIZE));
				closeedge(e);
				face(e0, e);
				face(e);
			}				
		}
		return 2;
	}
	else if (type == 20)
	{// 石柱
		vec o;
		{// 对其
			real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
			real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
			real us01 = (USIZE / (e[1] - e[0]).len());
			real us03 = (USIZE / (e[3] - e[0]).len());
			a01 = int(a01 / us01) * us01;
			a03 = int(a03 / us03) * us03;
			o = e[0] + (e[1] - e[0]) * a01 + (e[3] - e[0]) * a03;
		}
		real d = MIN((e[0] - e[1]).len(), (e[2] - e[1]).len());
		real h = MIN(hitdis, MIN(USIZE * 20, fixedSZ((1 + E) * d / E2)));
		real w = MIN(fixedSZ(h / 8), MAX(USIZE, d));
		vec dv = getedgenorm(e);
		if (abs(dv.dot(updir)) > 0.8)
		{
			if (dv.dot(updir) > 0)
				zhu(o, o + dv * (h), (0.5 * w));
			else
				zhu(o + dv * (h), o, (0.5 * w));
		}
		return 2;
	}	
	else if (type == 21)
	{// 拱门
		vec dv = getedgenorm(e);
		if (dv.dot(updir) > 0.8)
		{
			real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
			real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
			if (abs(a01 - 0.5) < abs(a03 - 0.5))
			{
				real d = MIN((e[0] - e[1]).len(), (e[2] - e[1]).len());
				real h = MIN(hitdis, MIN(USIZE * 20, fixedSZ((1 + E) * d / E2)));
				if (a03 < 0.5)
					gongmen(blend(e[0], e[2], 0.1), blend(e[1], e[3], 0.1), getedgenorm(e), h);
				else
					gongmen(blend(e[2], e[0], 0.1), blend(e[3], e[1], 0.1), getedgenorm(e), h);
			}
			else
			{
				real d = MIN((e[0] - e[1]).len(), (e[2] - e[1]).len());
				real h = MIN(hitdis, MIN(USIZE * 20, fixedSZ((1 + E) * d / E2)));
				if (a01 < 0.5)
					gongmen(blend(e[3], e[1], 0.1), blend(e[0], e[2], 0.1), getedgenorm(e), h);
				else
					gongmen(blend(e[2], e[0], 0.1), blend(e[1], e[3], 0.1), getedgenorm(e), h);
			}
		}
		else if (abs(dv.dot(updir)) < 0.2)
		{
			real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
			real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
			if (abs(a01 - 0.5) < abs(a03 - 0.5))
			{
				if (a03 < 0.5)
					gongmentop_frmside(e[0], e[1], getedgenorm(e), vec::UY);
				else
					gongmentop_frmside(e[2], e[3], getedgenorm(e), vec::UY);
			}
			else
			{
				if (a01 < 0.5)
					gongmentop_frmside(e[3], e[0], getedgenorm(e), vec::UY);
				else
					gongmentop_frmside(e[2], e[1], getedgenorm(e), vec::UY);
			}
		}
		return 2;
	}

	else if (type == 22)
	{// 随机面
		int lum = rrnd(200, 255);
		color = RGB(lum, lum, 255);
		bool ret = wallrnd(e[0], e[1], e[2], e[3]);
		color = 0xFFFFFFFF;
		return ret;
	}
	else if (type == 23)
	{// 分割面
		real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		
		moveedge(e, getedgenorm(e) * (USIZE * 0.1));
		bool ret = wall1(e[0], e[1], e[2], e[3], a01, a03, true, 0);
		color = 0xFFFFFFFF;
		return ret;
	}
	else if (type == 24)
	{// 方柱
		real d = MIN((e[0] - e[1]).len(), (e[2] - e[1]).len());
		real h = hitdis * EXPORT_SCALE < 50 ? hitdis : MIN(hitdis, MIN(USIZE * 10, fixedSZ((1 + E) * d / E2)));
		//real w = MIN(fixedSZ(h / 18), MAX(USIZE, d * E6));
		real w = MAX(USIZE, d * E6);
		vec o;
		{// 对其
			real uz = 0.5 * w;
			real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
			real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
			real us01 = (uz / (e[1] - e[0]).len());
			real us03 = (uz / (e[3] - e[0]).len());
			a01 = int(a01 / us01 + 0.5) * us01;
			a03 = int(a03 / us03 + 0.5) * us03;
			o = e[0] + (e[1] - e[0]) * a01 + (e[3] - e[0]) * a03;
		}
		vec dv = getedgenorm(e);
		if (abs(dv.dot(updir)) > 0.8)
		{
			fangzhu(o, o + dv * (h), (e[1] - e[0]).normcopy(), (e[3] - e[0]).normcopy(), (0.5 * w));
		}
		return 2;
	}
	else if (type == 25)
	{// 方拱门
		vec dv = getedgenorm(e);
		if (dv.dot(updir) > 0.8)
		{
			real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
			real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
			if (abs(a01 - 0.5) < abs(a03 - 0.5))
			{
				if (a03 < 0.5)
					fanggongmentop(e[0], e[1], getedgenorm(e));
				else
					fanggongmentop(e[2], e[3], getedgenorm(e));
			}
			else
			{
				if (a01 < 0.5)
					fanggongmentop(e[3], e[0], getedgenorm(e));
				else
					fanggongmentop(e[2], e[1], getedgenorm(e));
			}
		}
		else if (abs(dv.dot(updir)) < 0.2)
		{
			real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
			real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
			if (abs(e[0].p.y - e[1].p.y) < abs(e[2].p.y - e[1].p.y))
				if (e[0].p.y < e[2].p.y)
					fanggongmentop_frmside(a01 < 0.5 ? (e[0]) : (e[1]), (e[2] + e[3]) / 2, getedgenorm(e), vec::UY, a01 < 0.5 ? 1 : -1);
					//rectdoor(e[0], e[1], e[2], e[3]);
				else
					fanggongmentop_frmside(a01 > 0.5 ? (e[2]) : (e[3]), (e[0] + e[1]) / 2, getedgenorm(e), vec::UY, a01 > 0.5 ? 1 : -1);
					//rectdoor(e[2], e[3], e[0], e[1]);
			else
				if (e[1].p.y < e[3].p.y)
					fanggongmentop_frmside(a03 < 0.5 ? (e[1]) : (e[2]), (e[3] + e[0]) / 2, getedgenorm(e), vec::UY, a03 < 0.5 ? 1 : -1);
					//rectdoor(e[1], e[2], e[3], e[0]);
				else
					fanggongmentop_frmside(a03 < 0.5 ? (e[0]) : (e[3]), (e[1] + e[2]) / 2, getedgenorm(e), vec::UY, a03 > 0.5 ? 1 : -1);
					//rectdoor(e[3], e[0], e[1], e[2]);			
		}		
		return 2;
	}

	else if (type == 26)
	{// 小块
		vec o;
		{// 对其
			real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
			real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
			real us01 = (USIZE / (e[1] - e[0]).len());
			real us03 = (USIZE / (e[3] - e[0]).len());
			a01 = int(a01 / us01 + 0.5) * us01;
			a03 = int(a03 / us03 + 0.5) * us03;
			o = e[0] + (e[1] - e[0]) * a01 + (e[3] - e[0]) * a03;
		}
		int lum = rrnd(200, 255);
		color = RGB(lum, lum, 255);
		//real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		//real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		BLOCK2(o, (e[1] - e[0]).normcopy(), (e[3] - e[0]).normcopy());
		
		color = 0xFFFFFFFF;
		return 2;
	}
	else if (type == 27)
	{
		real a01 = (startpos - e[0]).dot((e[1] - e[0]).normcopy()) / (e[1] - e[0]).len();
		a01 = abs(a01 - 0.5) / 0.5;
		real a03 = (startpos - e[0]).dot((e[3] - e[0]).normcopy()) / (e[3] - e[0]).len();
		a03 = abs(a03 - 0.5) / 0.5;
		real d = MIN((e[0] - e[1]).len(), (e[2] - e[1]).len());
		closeedge(e);
		if (MIN((e[0] - e[1]).len(), (e[1] - e[2]).len()) > USIZE &&
			a01 < 0.5 && a03 < 0.5)
		{// 缩小
			if(a01 > 0.25 || a03 > 0.25)
			{
				extrudeedgeex(e, fixedSZ(d / 2), 0.5);
				face(e);
			}
			else
			{
				extrudeedgeex(e, USIZE, 0.5);
				face(e);
			}
		}
		else
		{// 放大	
			if (a01 < 0.75 && a03 < 0.75)
			{
				extrudeedgeex(e, fixedSZ(d / 2), 2);
				face(e);
			}
			else
			{
				extrudeedgeex(e, USIZE, 2);
				face(e);
			}
		}
		return true;
	}	
	else if (type == 29)
	{// 圆形窗
		roundwindow(e[0], e[1], e[2], e[3]);
		return true;
	}
	else if (type == 30)
	{// WATER
		color = 0xFFFF0000;
		extrudeedge(e, USIZE / 2);
		plane(e[0], e[1], e[2], e[3]);
		color = 0xFFFFFFFF;
		return 4;
	}
	else if (type == 31)
	{// 房檐
		color = blendcor(0xFF000080, 1, rrnd());
		int ind = 0;
		//LOGMSG("房檐")
		for (int ii = 0; ii < 4; ii ++){
			if (e[ii % 4].p.y > e[(ii + 1) % 4].p.y && abs(e[(ii + 1) % 4].p.y - e[(ii + 2) % 4].p.y) < USIZE * 0.1){
				ind = (ii + 1) % 4;
				//LOGMSG("ind = " << ind)
				break;
			}
		}
		real len = abs(e[(ind + 1) % 4].p.y - e[(ind + 2) % 4].p.y) * 1.732 / 2;
		vec n = getedgenorm(e);
		VECLIST oe;
		for (int i = 0; i < 4; i ++)
		{
			oe.push_back(e[(ind + i) % 4]);
		}
		e[ind].p += n * len;
		e[(ind + 1) % 4].p += n * len;
		vec side = e[(ind + 1) % 4].p - e[ind].p; side.norm();
		e[ind].p -= side * len;
		e[(ind + 1) % 4].p += side * len;
		//LOGMSG("len = " << len)
		//extrudeedge(e, USIZE / 2);
		VECLIST oe2;
		for (int i = 0; i < 4; i++)
		{
			oe2.push_back(e[(ind + i) % 4]);
		}
		face(oe, oe2);
		face(e);
		color = 0xFFFFFFFF;
		return 1;
	}
	else if (type == 32)
	{// MUD
		color = 0xFF001050;
		closeedge(e);
		extrudeedgeex(e, USIZE / 2, 1);
		plane(e[0], e[1], e[2], e[3]);
		color = 0xFFFFFFFF;	
		//flowerplane(e[0], e[1], e[2], e[3]);
		return 6;
	}
	else if (type == 33)
	{// 方形窗
		if (abs(e[0].p.y - e[1].p.y) < abs(e[2].p.y - e[1].p.y))
			fangwindow(e[0], e[1], e[2], e[3], 3, 3);
		else
			fangwindow(e[1], e[2], e[3], e[0], 3, 3);
		return true;
	}
	else if (type == 34)
	{// 田形窗
		if (abs(e[0].p.y - e[1].p.y) < abs(e[2].p.y - e[1].p.y))
			tianwindow(e[0], e[1], e[2], e[3], 4, 4);
		else
			tianwindow(e[1], e[2], e[3], e[0], 4, 4);
		return true;
	}
	else if (type == 35)
	{// 花毯
		color = 0xFF003060;
		extrudeedgeex(e, USIZE / 2, 1);
		plane(e[0], e[1], e[2], e[3]);
		color = 0xFFFFFFFF;
		flowerplane(e[0], e[1], e[2], e[3]);
		return 6;
	}
	return false;
}
// --------------------------------------------------------
// 植物造型艺术
// --------------------------------------------------------
extern "C"
{
	bool EXPORT_API araceae(int type, vec o, vec v, real s,
		int n,
		int aa,
		int an
		)
	{
		gcommonvertex = 1;
		START_MESH
		//{// 加一个土底
		//	real s1 = s * 10;
		//	VECLIST e;
		//	roundedge(e, o, vec::UY, s1, 8, PI / 8);
		//	color = blendcor(0xFFFFFFFF, 0xFF5A4EAA, 0.25);
		//	extrudeedgeex(e, s1 * E4, E);
		//	color = blendcor(0xFFFFFFFF, 0xFF5A4EAA, 0.5);
		//	face(e, getedgecenter(e));
		//	color = 0xFFFFFFFF;
		//}
		if (type == 0)
		{
			curve(o, v, s);
			color = 0xFFFFFFFF;
		}
		else if (type == 1)
		{
			Araceae1(o, v, s);
		}
		else if (type == 2)
		{
			Araceae3(o, v, s);
		}
		else if (type == 3)
		{
			Araceae4(o, v, s);
		}
		else if (type == 4)
		{// 竹子
			bamboo(o, vec::UY, s);
		}
		else if (type == 5)
		{// 棕榈树
			PalmTree(o, vec::UY, s);				
		}
		else if (type == 6)
		{// 松柏
			//ziteng(o, v, s);
			songbai(o, vec::UY, s);
		}
		else if (type == 7)
		{// 冬青
			dongqing(o, vec::UY, s);
		}
		else if (type == 8)
		{// 树
			PlumTree(o, vec::UY, s);
		}
		END_MESH
			gcommonvertex = 0;
		return true;
	}
}
// ---------------------------------------------------------
// UIPM
// ---------------------------------------------------------
extern "C"
{
	// 机械手臂
	bool EXPORT_API robotarm(int type, vec v1, vec v2, real len1, real len2, real t)
	{
		if (type == 1)
		{
			START_MESH
			gtimealpha = t;
			robotarm(vec::CENTER, v1, v2, 0.01, len1 / EXPORT_SCALE, len2 / EXPORT_SCALE);
			
			END_MESH
			return true;
		}
		return false;
	}
	// 3D 直线
	bool EXPORT_API line3D(vec p1, vec p2, vec cam, real wid)
	{
		START_MESH
		bool result = line3d(vec(p1.x, p1.y, p1.z) / EXPORT_SCALE, vec(p2.x, p2.y, p2.z) / EXPORT_SCALE, vec(cam.x, cam.y, cam.z) / EXPORT_SCALE, wid);
		END_MESH
		return result;
	}
	// --------------------------------------------------------
	// 动物
	// --------------------------------------------------------
	// worm
	bool EXPORT_API worm(int ind, vec p1, vec p2, real t, real sz = 0.005)
	{
		START_MESH
		
		gtimealpha = t;
		for (int i = 0; i < 8; i ++)
			mudbug1(ind + i, p1 / EXPORT_SCALE, p2 / EXPORT_SCALE, 8, sz);

		END_MESH
		return true;
	}
	// Coelenterate
	bool EXPORT_API coelenterate(vec o, vec v, real s, real t,
		int n, 	// 体节数
		int aa, // 触角数
		int an  // 触角节数
		)
	{
		START_MESH	
		gtimealpha = t;
		Coelenterate(o / EXPORT_SCALE, v, s,
			n,
			aa,
			an
			);
		END_MESH
		return true;
	}
	
	// --------------------------------------------------------
	// growing plants
	bool EXPORT_API growing_plant(int type, vec o, vec v, real s, real t)
	{
		gcommonvertex = 1;
		START_MESH
		if (type == 0)
		{
			growing_plant(o, v, s, t);
		}
			
		END_MESH
		gcommonvertex = 0;
		return true;
	}
}
// ---------------------------------------------------------
// setup
// ---------------------------------------------------------
#include "phg/realphg.hpp"
extern "C"
{
	void EXPORT_API setup()
	{
		RealPHG::setup();
	}
}