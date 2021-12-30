using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

// *******************************************************
// 公共接口
// *******************************************************
public class Common
{    
    public static string sceneName = "s18";   
	public static string language = "en";
	public static int VER = 2;							// 最新版本号
	public static int version = VER;                    // 版本号
	
    public static bool bshowmenu = false;
    public static bool gamepaused = false;
	public static int gamestage = 0;
	public static float lstmousedown = 0;
	public static float camdis = 0.0f;
	public static float starttime = 0.0f;
	
    /// 拖拽
    static Action enddraggingAction;   
	public delegate GameObject DragingGoHandler();
    static DragingGoHandler draginggohdr;
    public static bool isdragging()
    {
        return draginggohdr != null;
    }
    public static void dragbegin(DragingGoHandler draginggohandler, Action enddraggingaction )
    {
        Debug.Log("dragbegin");
        draginggohdr = draginggohandler;
        enddraggingAction = enddraggingaction;
    }
    public static void dragend()
    {
        Debug.Log("dragend");
        draginggohdr = null;
        if (enddraggingAction != null)
            enddraggingAction();
        enddraggingAction = null;
    }
    public static void dragcanceled()
    {
        Debug.Log("dragcanceled");
        draginggohdr = null;
        enddraggingAction = null;
    }
    public static GameObject draginggo()
    {
        if (draginggohdr != null)
        {
            return draginggohdr();
        }
        return null;
    }

    /// 点选
    public static bool treated = false;
    static Ray lstray;
    static RaycastHit lsthit;
    static Ray lstray0;
    static RaycastHit lsthit0;
    static Ray lstray1;
    static Ray lstray2;
    static RaycastHit lsthit2;
    public static void settreated(bool b, GameObject go, int id = 0)
    {
        if(go != null)
            Debug.Log("settreated : " + go.name + " " + id.ToString());
        treated = b;
    }
    public static void clearlstselection()
    {
        lsthit = new RaycastHit();
    }
    public static bool mouseselect(Ray ray, out RaycastHit rhit, float maxDis = 100.0f)
    {
        if (lstray.Equals(ray) && lsthit.collider && lsthit.collider.gameObject)
        {
            // Debug.Log("mouseselect lstray.Equals");
            rhit = lsthit;
            return true;
        }
        treated = false;
        int layerMask = 1 << 8; // 不包括
        if (Physics.Raycast(ray, out rhit, maxDis, ~layerMask))
        //    if (Physics.Raycast(ray, out rhit, maxDis))
        {
            //  Debug.Log("mouseselect Physics.Raycast " + rhit.collider.gameObject.ToString()); 
            lstray = ray;
            lsthit = rhit;
            return true;
        }
        return false;
    }
    public static bool mouseselect0(Ray ray, out RaycastHit rhit, float maxDis = 100.0f)
    {        
        if (lstray0.Equals(ray) && lsthit0.collider && lsthit0.collider.gameObject)
        {
            // Debug.Log("mouseselect lstray.Equals");
            rhit = lsthit0;
            return true;
        }
        treated = false;
        if (Physics.Raycast(ray, out rhit, maxDis))
        {
            //  Debug.Log("mouseselect Physics.Raycast " + rhit.collider.gameObject.ToString()); 
            lstray0 = ray;
            lsthit0 = rhit;
            return true;
        }
        return false;
    }
    public static bool rayIsHit(Ray ray, float maxDis = 100.0f)
    {
        if (lstray1.Equals(ray))
        {
            return true;
        }
        int layerMask = 1 << 8; // 不包括地表
        if (Physics.Raycast(ray, maxDis, ~layerMask))
        {
            lstray1 = ray;
            return true;
        }
        return false;
    }
    public static bool rayHitTest(Ray ray, out RaycastHit rhit, float maxDis = 100.0f)
    {
        if (lstray2.Equals(ray) && lsthit2.collider && lsthit2.collider.gameObject)
        {
            rhit = lsthit2;
            return true;
        }
        int layerMask = 1 << 8; // 不包括
        //if (Physics.Raycast(ray, out rhit, maxDis, ~layerMask))
        if (Physics.Raycast(ray, out rhit, maxDis, ~layerMask))
        {
            lstray2 = ray;
            lsthit2 = rhit;
            return true;
        }
        return false;
    }
    /// 屏幕消息
    public static void screenprint(string content)
    {
        Debug.Log("screenprint: " + content);
    }
    /// 错误消息
    public static void debugerr(string content)
    {
        Debug.Log("error: " + content);
    }
}
// *******************************************************
// 延时执行
// *******************************************************
public class DelayToInvoke : MonoBehaviour
{
    public static IEnumerator Invoke(Action action, float delaySeconds)
    {
        yield return new WaitForSeconds(delaySeconds);
        action();
    }
}
// *******************************************************
// 静态数值 - 道具
// *******************************************************
public class ItemStaticData
{
    /// 矿物列表
    public static string[] sMineName = new string[]{
    "磷矿",
    "萤石矿",
    "石灰岩",
    "云母矿",
    "橄榄石",
    "重晶石",
    "电气石",
    "绿泥石",
    "白云石",
    "拉长石",
    "钴华",
    "钙铀云母",
    "钠沸石",
    "透长石",
    "白云石",
    "透辉石",
    "蛇纹石",
    "黑电气石",
    "钠长石",
    "硬石膏",
    "菱镁矿",
    "碳酸钡矿",
    "方柱石",
    "臭葱石",
    "高岭石",
    "硅孔雀石",
    "钙长石",
    "蔷薇辉石",
    "铜铀云母",
    "蓝晶石",
    "普通辉石",
    "铬铅矿",
    "菱铁矿",
    "白钨矿",
    "独居石",
    "钙铁榴石",
    "白云母",
    "菱锌矿",
    "黄钾铁矾",
    "绿帘石",
    "霞石",
    "红柱石",
    "堇青石",
    "正长石",
    "古铜辉石",
    "磷灰石",
    "夕线石",
    "叶腊石",
    "锂电气石",
    "黝帘石",
    "微斜长石",
    "文石",
    "镍华",
    "普通角闪石",
    "白榴石",
    "方钠石",
    "顽火辉石",
    "钙铁辉石",
    "紫苏辉石",
    "白铅矿",
    "中长石",
    "钒铅矿",
    "香花石",
    "氟钙铈矿",
    "碳酸锶矿",
    "铁铝榴石",
    "钼铅矿",
    "磷氯铅矿",
    "奥长石",
    "蓝铁矿",
    "硬绿泥石",
    "绿泥石",
    "刚玉",
    "钻石",
    "滑石",
    "方解石",
    "黄玉",
    "紫水晶",
    "猫眼石",
    "蛋白石",
    "蛭石",
    };
    /// <summary>
    ///  合成公式
    /// </summary>
    public static string[,] rawmap = new string[,]{
    {"房间","菱锌矿","萤石矿","","","菱铁矿","",},
    {"平台","黄钾铁矾","石灰岩","","","白钨矿","",},
    {"延伸","绿帘石","云母矿","","","独居石","",},
    {"平面","霞石","橄榄石","","","钙铁榴石","",},
    {"方凸","红柱石","重晶石","","","白云母","",},
    {"斜凸","堇青石","电气石","","","菱锌矿","",},
    {"屋顶","正长石","绿泥石","","","黄钾铁矾","",},
    {"窗","古铜辉石","白云石","","","绿帘石","",},
    {"门","磷灰石","拉长石","","","霞石","",},
    {"台阶","夕线石","钴华","","","红柱石","",},
    {"栅栏","叶腊石","钙铀云母","","","堇青石","",},
    {"合成器","白榴石","钠长石","","","黝帘石","",},
    {"小车","方钠石","硬石膏","","","微斜长石","",},
    {"种子包","顽火辉石","菱镁矿","","","文石","",},
    {"手电","钙铁辉石","碳酸钡矿","","","镍华","",},
    {"灯笼","紫苏辉石","方柱石","","","普通角闪石","",},
    {"路灯","白铅矿","臭葱石","","","白榴石","",},
    {"多肉","钙铁辉石","碳酸钡矿","","","镍华","",},
    {"芦苇","紫苏辉石","方柱石","","","普通角闪石","",},
    {"庄稼","白铅矿","臭葱石","","","白榴石","",},
    {"土虫","中长石","高岭石","","","方钠石","",},
    {"肥料","钒铅矿","硅孔雀石","","","顽火辉石","",},
    {"随机窗","氟钙铈矿","蔷薇辉石","","","紫苏辉石","",},
    {"日形窗","碳酸锶矿","铜铀云母","","","白铅矿","",},
    {"方形门","钼铅矿","普通辉石","","","钒铅矿","",},
    {"拱形门","磷氯铅矿","铬铅矿","","","香花石","",},
    };

    /// <summary>
    /// 道具表
    /// </summary>
    /// <param name="itemmap"></param>
    public static void setupItmDat(out Dictionary<string, ArrayList> itemmap)
    {
        itemmap = new Dictionary<string, ArrayList>();
        // 总分类
        itemmap["root"] = new ArrayList(new[] { "平台"});//, "岩石", "水", "土" "种植", "器具", "魔方" 
        // 一级 
        itemmap["种植"] = new ArrayList(new[] { "水草", "荷花", "松柏", "冬青", "花毯" }); // "棕榈树", "多肉", "芦苇", "盆栽", 
        itemmap["建筑"] = new ArrayList(new[] { "面", "屋顶", "门窗", "连接", "廊柱" });
        itemmap["器具"] = new ArrayList(new[] { "路灯", "栅栏"});//"喷泉", "合成器", "小车", "种子包","手电"，"灯笼", "轨道车",, "占星" 
        itemmap["色彩"] = new ArrayList(new[] { "红", "绿", "蓝", "黑", "白", "涂鸦", "雕塑" });//"黄", 
        itemmap["摧毁"] = new ArrayList(new[] { "删除", "恢复" });
        // 二级
        itemmap["面"]   = new ArrayList(new[] { "面块", "斜面", "块", "缩放" });
        itemmap["廊柱"] = new ArrayList(new[] { "方柱", "方拱门" });//,"圆柱","拱门"}); 
        itemmap["屋顶"] = new ArrayList(new[] { "尖顶", "圆顶", "平顶" });// "房檐"
        itemmap["门窗"] = new ArrayList(new[] { "随机窗", "方窗","日形窗", "田字窗", "圆形窗", "方形门", "拱形门"});
        itemmap["连接"] = new ArrayList(new[] { "园墙", "石阶", "台阶", "上阶" });       
    }
}
// *******************************************************
// 静态数值 - 地理学
// *******************************************************
public class GeoStaticData
{
    // 常数
    public static float R0 = 434.25F / 4;
    public static Vector3 PlanetCenter = new Vector3(0, -GeoStaticData.R0, 0);
    public static Vector3 getup(Vector3 p)
    {
        return Vector3.up;
      //  Vector3 o = new Vector3(0, -GeoStaticData.R0, 0);
     //   return Vector3.Normalize(p - o);
    }
    public static Vector3 getspherepos(Vector3 p, float h0 = 5.0f)
    {
        return new Vector3(p.x, h0, p.z);
        //Vector3 o = new Vector3(0, -GeoStaticData.R0, 0);
       // Vector3 n = Vector3.Normalize(p - o);
       // return o + n * (GeoStaticData.R0 + h0);
    }
    // --------------------------------
    static bool checkPointInTriangle(
        Vector3 point,
        Vector3 a, Vector3 b, Vector3 c,
        float tolerance = 0.05f)
    {
        float total_angles = 0.0f;

        // make the 3 vectors
        Vector3 v1 = new Vector3(point.x - a.x, point.y - a.y, point.z - a.z); v1.Normalize();
        Vector3 v2 = new Vector3(point.x - b.x, point.y - b.y, point.z - b.z); v2.Normalize();
        Vector3 v3 = new Vector3(point.x - c.x, point.y - c.y, point.z - c.z); v3.Normalize();

        float Dot1 = Vector3.Dot(v2, v1);
        if (Dot1 < -1.0f)
            Dot1 = -1.0f;
        if (Dot1 > 1.0f)
            Dot1 = 1.0f;
        total_angles += Mathf.Acos(Dot1);

        float Dot2 = Vector3.Dot(v3, v2);
        if (Dot2 < -1.0f)
            Dot2 = -1.0f;
        if (Dot2 > 1.0f)
            Dot2 = 1.0f;
        total_angles += Mathf.Acos(Dot2);

        float Dot3 = Vector3.Dot(v1, v3);
        if (Dot3 < -1.0f)
            Dot3 = -1.0f;
        if (Dot3 > 1.0f)
            Dot3 = 1.0f;
        total_angles += Mathf.Acos(Dot3);

        // Debug.Log("point " + point.ToString() + " a " + a.ToString() + " b " + b.ToString() + " c " + c.ToString());
        // Debug.Log("total_angles " + total_angles);
        if (Mathf.Abs(total_angles - 2.0f * Mathf.PI) <= tolerance)
        {
            //Debug.Log("total_angles " + total_angles);
            return true;
        }
        return false;
    }
  
    public static Quaternion getGoundRot(Vector3 pos)
    {
        //Vector3 op = pos - Vector3.zero;//len
        //if (op.magnitude > 0.1)
        //{
        //    Vector3 oo = new Vector3(0, -GeoStaticData.R0, 0);
        //    Vector3 oop = pos - oo;
        //    Vector3 dx = Vector3.Cross(oop, op);
        //    return Quaternion.LookRotation(Vector3.Cross(oop, dx).normalized, oop.normalized);
        //}
        return Quaternion.identity;
    }
   

}
