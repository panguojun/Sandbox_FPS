using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine.Networking;
using System.Net;
// --------------------------------
// PICK
// --------------------------------
public class PICK : MonoBehaviour
{
    public static Color s_brushcolor = Color.white;	
    public GameObject MUD;
    public GameObject WATER;
    public GameObject BUILDING;

    public GameObject Preview;
	
	GameObject MOON;

    RaycastHit lsthit;
    float lstdis0 = -1;

    Vector3[] lstvertices;
    int[] lsttriangles;
    GameObject lstAddedObj;
    Color32[] lstcolors32;
    int lstOperType = 0;
    // --------------------------------
    void Start()
    {
        if (Preview)
            Preview.SetActive(false);
		MOON = GameObject.Find("scene");
    }
	
	GameObject _Instantiate(GameObject g)
	{
		GameObject ng = Instantiate(g);
		ng.transform.parent = MOON.transform;
		return ng;
	}
    // --------------------------------
    // GUI
    // --------------------------------
    public void lsthittest(int type)
    {
        hittest(type, lsthit, false, lstdis0);
    }
    public void hittest(Ray ray, int type, bool bpriview = false, float dis0 = -1)
    {
        if (Common.mouseselect(ray, out lsthit) && lsthit.collider.gameObject == gameObject)
        {
            if (type < 1000)
            {
                hittest(type, lsthit, bpriview, dis0);
                lstdis0 = dis0;
            }
            else
            {// 色彩
                colorface(type, lsthit);
            }
        }
    }
    void colorface(int type, RaycastHit hit)
    {
        if (hit.collider.gameObject == gameObject)
        {
            MeshCollider collider = hit.collider as MeshCollider;
            if (collider == null || collider.sharedMesh == null)
                return;
            //获取碰撞器所在物体的Mesh网格
            Mesh mesh0 = collider.sharedMesh;
            //获取Mesh网格的所有顶点
            Vector3[] vertices = mesh0.vertices;
            lstcolors32 = mesh0.colors32;
            Color32[] colors32 = mesh0.colors32;
            int hittriangleIndex = hit.triangleIndex * 3;
            // 传递参数
            //获取mesh的三角形索引，这里的索引的就是模型顶点数组的下标
            int[] triangles = mesh0.triangles;
            int ind = PMDLL.getnexttri(vertices, vertices.Length, triangles, triangles.Length, hittriangleIndex);
            //if (ind >= 0)
            {
                PMDLL.colorface(colors32, vertices.Length, triangles, triangles.Length, hittriangleIndex, ind, new Color32((byte)(s_brushcolor.r * 255), (byte)(s_brushcolor.g * 255), (byte)(s_brushcolor.b * 255), (byte)(s_brushcolor.a * 255)));
                {
                    gameObject.SetActive(false);
                    mesh0.colors32 = colors32;
                    MeshFilter mf = gameObject.GetComponent<MeshFilter>();
                    mf.mesh = mesh0;
                    gameObject.SetActive(true);
                }
            }
            lstOperType = 100;
        }        
    }
    void hittest(int type, RaycastHit hit, bool bpriview, float dis0)
    {    
        if (lsthit.collider && hit.collider.gameObject == gameObject)
        {
           // Preview.transform.position = hit.point;
            //Debug.Log("Raycast");
            MeshCollider collider = hit.collider as MeshCollider;
            if (collider == null || collider.sharedMesh == null)
                return;
            // 传递参数
            float hitdis = 100.0f;
            //获取碰撞器所在物体的Mesh网格
            Mesh mesh0 = collider.sharedMesh;
            //获取Mesh网格的所有顶点
            if(!bpriview)
                lstvertices = mesh0.vertices;
            Vector3[] vertices = mesh0.vertices;
            
            //获取mesh的三角形索引，这里的索引的就是模型顶点数组的下标
            if (!bpriview)
                lsttriangles = mesh0.triangles;            
            int[] triangles = mesh0.triangles;
            int hittriangleIndex = hit.triangleIndex * 3;
            if (hittriangleIndex >= 0 && hittriangleIndex < triangles.Length)
            {// 由三角形索引得到方形面 
                //Debug.Log("hittriangleIndex = " + hittriangleIndex.ToString());
                {// 合法性判定
                    Vector3 p1 = vertices[triangles[hittriangleIndex]];     
                    Vector3 p2 = vertices[triangles[hittriangleIndex + 1]]; 
                    Vector3 p3 = vertices[triangles[hittriangleIndex + 2]]; 
                    // Preview.transform.position = (p1 + p2 + p3) / 3.0f;                    
                    Vector3 norm = Vector3.Cross((p2 - p1), (p3 - p1));
                    if(norm.magnitude < 1E-06)
                    {
                        Debug.Log("(norm.magnitudea = 0!");
                        return;
                    }
                    //  Debug.Log("norm = " + norm.ToString());
                    float facedeta = 0.0001f;
                    float facedir = 1;
                    if(type == 4 || type == 7)
                    {
                        facedeta = -0.001f;
                        facedir = -1;
                    }
                    { // 碰撞点上测试法线方向的距离
                        int layerMask = 1 << 8; // 不包括地表
                        Vector3 pm = hit.point + hit.normal * facedeta;
                        RaycastHit hit2;
                        if(Physics.Raycast(new Ray(pm, hit.normal * facedir), out hit2, hitdis, ~layerMask))
                        {
                            if(!bpriview)
                                Debug.Log("碰撞点测试法线方向的距离: " + hit2.distance.ToString());
                            hitdis = hit2.distance;
                        }
                    }
					const float ustep = 0.1f;
                    if (dis0 > 0 && dis0 < hitdis)
                        hitdis = Mathf.FloorToInt(dis0 / ustep + 0.5f) * ustep;
                }                
                bool newbuilding = false;
                int edited = 0;
                int ind = PMDLL.getnexttri(vertices, vertices.Length, triangles, triangles.Length, hittriangleIndex);			
				
                if (ind == -1)
                {
                    edited = PMDLL.editfacetri(type + (bpriview ? 1000 : 0), vertices, vertices.Length, triangles, triangles.Length, mesh0.colors32, hittriangleIndex,
                        collider.transform.InverseTransformPoint(Camera.main.transform.position),
                        collider.transform.InverseTransformPoint(hit.point),
                        collider.transform.InverseTransformDirection(GeoStaticData.getup(hit.point)),
                        hitdis / transform.localScale.x // 缩放
                        );
                }
                else
                {
                    if (type == 0 && vertices.Length > 1000)
                    {
                        newbuilding = true;
                        type = 12;
                    }                   
                    edited = PMDLL.editface(type + (bpriview ? 1000 : 0), vertices, vertices.Length, triangles, triangles.Length, mesh0.colors32, hittriangleIndex, ind,
                        collider.transform.InverseTransformPoint(Camera.main.transform.position),
                        collider.transform.InverseTransformPoint(hit.point),
                        collider.transform.InverseTransformDirection(GeoStaticData.getup(hit.point)),
                        hitdis / transform.localScale.x // 缩放
                        );
                    
                    if (!bpriview && edited == 1 && (type != 0) && !newbuilding)
                    {// 隐藏面
                        Vector3 p1 = vertices[triangles[ind]];
                        Vector3 p2 = vertices[triangles[ind + 1]];
                        Vector3 p3 = vertices[triangles[ind + 2]];
                        vertices[triangles[ind + 1]] = Vector3.Slerp(p1, p2, 0.01F);
                        vertices[triangles[hittriangleIndex + 1]] = Vector3.Slerp(vertices[triangles[hittriangleIndex]], vertices[triangles[hittriangleIndex + 1]], 0.001F);
                        vertices[triangles[ind + 2]] = Vector3.Slerp(p1, p3, 0.01F);
                        vertices[triangles[hittriangleIndex + 2]] = Vector3.Slerp(vertices[triangles[hittriangleIndex]], vertices[triangles[hittriangleIndex + 2]], 0.001F);
                    }
                    if (!bpriview && (edited == 3 || edited == 1) && !newbuilding)
                        mesh0.vertices = vertices;
                }                
                if (edited > 0)
                {
                    if (edited == 5
                        || type == 13 // 栅栏
                        || type == 11 || type == 12 || type == 26 // 块
                        || type == 24 || type == 25 // 拱门
                        || type == 8 || type == 17 || type == 18 || type == 31 // 屋顶
                        || type >= 14 && type <= 16 && !(gameObject.name.Contains("_14_") || gameObject.name.Contains("_15_") || gameObject.name.Contains("_16_")) // 楼梯
                        || type == 19 && !(gameObject.name.Contains("_19_")) // 连接
                        )
                    {// 添加模型                       
                        if (bpriview)
                        {
                            Preview.SetActive(false);
                            MeshFilter Previewmf = Preview.GetComponent<MeshFilter>();
                            PMDLL.updateMeshEx(Previewmf.mesh);
                            Preview.SetActive(true);
                        }
                        else
                        {
                            GameObject bd = _Instantiate(BUILDING);
                            bd.name = "Building_" + type.ToString() + "_" + bd.GetInstanceID().ToString();
                            bd.SetActive(false);
                            MeshFilter mf = bd.GetComponent<MeshFilter>();
                            Mesh mesh = mf.mesh;
                            PMDLL.updateMeshEx(mesh);
                            mf.mesh = mesh;
                            MeshCollider mc = bd.GetComponent<MeshCollider>();
                            if (mc) mc.sharedMesh = mesh;
                            ((MeshCreator)bd.GetComponent(typeof(MeshCreator))).meshtype = -1;
                            bd.SetActive(true);
                            bd.transform.position = transform.position;                                                        
                            bd.transform.rotation = transform.rotation;
                            bd.transform.localScale = transform.localScale;
                            //bd.transform.SetParent(transform);
                            lstAddedObj = bd;
                            lstOperType = 2;                                                 
                        }
                    }
                    else if (edited == 3) // 延伸
                    {// 更新模型
                        gameObject.SetActive(false);
                        mesh0.RecalculateBounds();
                        mesh0.RecalculateNormals();
                        MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
                        if (mc) mc.sharedMesh = mesh0;
                        MeshFilter mf = gameObject.GetComponent<MeshFilter>();
                        mf.mesh = mesh0;
                        gameObject.SetActive(true);
                        lstOperType = 1;
                    }
                    else if(edited == 4)
                    {// 添加水面
                        if (bpriview)
                        {
                            Preview.SetActive(false);
                            MeshFilter Previewmf = Preview.GetComponent<MeshFilter>();
                            PMDLL.updateMeshEx(Previewmf.mesh);
                            Preview.SetActive(true);
                        }
                        else
                        {
                            GameObject bd = _Instantiate(WATER);
                            bd.name = "WATER" + bd.GetInstanceID().ToString();
                            //bd.SetActive(false);
                            MeshFilter mf = bd.GetComponent<MeshFilter>();
                            Mesh mesh = mf.mesh;
                            PMDLL.updateMeshEx(mesh);
                            mf.mesh = mesh;
                            MeshCollider mc = bd.GetComponent<MeshCollider>();
                            if (mc) mc.sharedMesh = mesh;
                            bd.SetActive(true);
                            bd.transform.position = transform.position;
                            bd.transform.rotation = transform.rotation;
                            bd.transform.localScale = transform.localScale;
                            //bd.transform.SetParent(transform);
                            lstAddedObj = bd;
                            lstOperType = 2;
                        }                        
                    } 
                    else if (edited == 6)
                    {// 添加泥土
                        if (bpriview)
                        {
                            Preview.SetActive(false);
                            MeshFilter Previewmf = Preview.GetComponent<MeshFilter>();
                            PMDLL.updateMeshEx(Previewmf.mesh);
                            Preview.SetActive(true);
                        }
                        else
                        {
                            GameObject bd = _Instantiate(MUD);
                            bd.name = "MUD" + bd.GetInstanceID().ToString();
                            //bd.SetActive(false);
                            MeshFilter mf = bd.GetComponent<MeshFilter>();
                            Mesh mesh = mf.mesh;
                            PMDLL.updateMeshEx(mesh);
                            mf.mesh = mesh;
                            MeshCollider mc = bd.GetComponent<MeshCollider>();
                            if (mc) mc.sharedMesh = mesh;
                            bd.SetActive(true);
                            bd.transform.position = transform.position;
                            bd.transform.rotation = transform.rotation;
                            bd.transform.localScale = transform.localScale;
                            //bd.transform.SetParent(transform);
                            lstAddedObj = bd;
                            lstOperType = 2;
                        }                        
                    }
                    else
                    {// 改变模型
                        if (bpriview)
                        {
                            Preview.SetActive(false);
                            MeshFilter Previewmf = Preview.GetComponent<MeshFilter>();                            
                            PMDLL.updateMeshEx(Previewmf.mesh);
                            Preview.SetActive(true);                            
                        }
                        else
                        {
                            CombineInstance[] combine = new CombineInstance[2];

                            combine[0].mesh = mesh0;
                            combine[0].transform = Matrix4x4.identity;

                            combine[1].mesh = new Mesh();
                            combine[1].transform = Matrix4x4.identity;
                            if (PMDLL.updateMeshEx(combine[1].mesh))
                            {
                                gameObject.SetActive(false);
                                Mesh newmesh = new Mesh();
                                newmesh.CombineMeshes(combine);

                                MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
                                if (mc) mc.sharedMesh = newmesh;

                                MeshFilter mf = gameObject.GetComponent<MeshFilter>();
                                mf.mesh = newmesh;

                                gameObject.SetActive(true);                                
                            }
                        }
                        lstOperType = 1;
                    }                    
                }
            }
        }        
    }
    // --------------------------------
    public void undo(int type)
    {
        if(lstOperType == 1)
        {// 分割 延伸 等更改模型操作
            undoVertex();
        }
        else if (lstOperType == 2 && lstAddedObj != null)
        {// 添加部件方式
            DestroyObject(lstAddedObj);
            lstAddedObj = null;
        }
        else if(lstOperType == 100)
        {
            MeshFilter mf = gameObject.GetComponent<MeshFilter>();
            gameObject.SetActive(false);
            mf.mesh.colors32 = lstcolors32;
            gameObject.SetActive(true);
        }
    }
    // --------------------------------
    void undoVertex()
    {
        if (lstvertices == null)
            return;
        MeshFilter mf = gameObject.GetComponent<MeshFilter>();
        Mesh mesh0 = mf.mesh;
        
        mesh0.triangles = lsttriangles;
        mesh0.vertices = lstvertices;

        gameObject.SetActive(false);
        mesh0.RecalculateBounds();
        mesh0.RecalculateNormals();
        MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
        if (mc) mc.sharedMesh = mesh0;
        mf.mesh = mesh0;
        gameObject.SetActive(true);
    }    
    // --------------------------------
    public void clearpreview()
    {
        Preview.SetActive(false);
    }
    // --------------------------------
    public bool hasPreview()
    {
        return Preview.activeSelf;
    }
}