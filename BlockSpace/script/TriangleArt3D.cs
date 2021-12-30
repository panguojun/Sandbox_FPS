using UnityEngine;
using System.Collections;

public class TriangleArt3D : MonoBehaviour
{
    public GameObject portalbox;
    RaycastHit lsthit;
    Color32[] lstcolor32;
    Vector3[] lstvertices;
    Vector3 ld_mousepos;
    int[] lsttriangles;
    int lstedittype = -1;
    bool lstmousedownsucc = false;
    // --------------------------------
    // GUI
    // --------------------------------
    bool getpos(Ray ray, int type, bool bpriview = false)
    {
        if (Common.mouseselect(ray, out lsthit) && lsthit.collider.gameObject == gameObject)
        {
            MeshCollider collider = lsthit.collider as MeshCollider;
            if (collider == null || collider.sharedMesh == null)
                return false;
            return true;
        }
        return false;
    }
    void createTriangle(Ray ray, int type)
    {
        if (!lsthit.collider || lsthit.collider.gameObject != gameObject)
            return;
        {
            MeshCollider collider = lsthit.collider as MeshCollider;
            //获取碰撞器所在物体的Mesh网格
            Mesh mesh0 = collider.sharedMesh;
            //获取Mesh网格的所有顶点
            Vector3[] vertices = mesh0.vertices;
            Color32[] colors32 = mesh0.colors32;
            // 传递参数
            //获取mesh的三角形索引，这里的索引的就是模型顶点数组的下标
            int[] triangles = mesh0.triangles;
            int hittriangleIndex = lsthit.triangleIndex * 3;
            if (hittriangleIndex >= 0 && hittriangleIndex < triangles.Length)
            {// 由三角形索引得到方形面     
                //Debug.Log("由三角形索引得到方形面" + hittriangleIndex.ToString());
                {// 合法性判定
                    Vector3 p1 = vertices[triangles[hittriangleIndex]];// Debug.Log("p1 = " + p1.ToString());
                    Vector3 p2 = vertices[triangles[hittriangleIndex + 1]]; //Debug.Log("p2 = " + p2.ToString());
                    Vector3 p3 = vertices[triangles[hittriangleIndex + 2]];// Debug.Log("p3 = " + p3.ToString());
                    Plane hitplane = new Plane(transform.TransformPoint(p1), transform.TransformPoint(p2), transform.TransformPoint(p3));
                    float enter;
                    if (hitplane.Raycast(ray, out enter))
                    {
                        if (type == 1)
                        {
                            lstcolor32 = mesh0.colors32;
                            lstedittype = 1;
                            if (PMDLL.colorface(colors32, vertices.Length, triangles, triangles.Length, hittriangleIndex, -1, PMDLL.getUInt()))
                            {
                                collider.gameObject.SetActive(false);
                                mesh0.colors32 = colors32;
                                MeshFilter mf = collider.gameObject.GetComponent<MeshFilter>();
                                mf.mesh = mesh0;
                                collider.gameObject.SetActive(true);
                            }                      
                        }
                        else
                        {
                            Vector3 pp1 = Camera.main.WorldToScreenPoint(lsthit.point);
                            Vector3 pp2 = Camera.main.WorldToScreenPoint(lsthit.point + lsthit.normal * 1);
                            Vector3 dir = (pp2 - pp1).normalized;
                            Vector3 msdeta = (Input.mousePosition - ld_mousepos);
                            float dot = Vector3.Dot(msdeta, dir);
                            if (dot <= 0)
                                return;
                            Vector3 pos = lsthit.point + lsthit.normal * (dot * 0.0025f);
                            PMDLL.setVec(transform.InverseTransformPoint(lsthit.point));

                            int edited = PMDLL.triangleArt3D(type, vertices, vertices.Length, triangles, triangles.Length, mesh0.colors32, hittriangleIndex,
                                transform.InverseTransformPoint(pos)
                                );
                            if (edited == 2)
                            {
                                lstedittype = 2;
                                lstvertices = mesh0.vertices;

                                collider.gameObject.SetActive(false);
                                mesh0.vertices = vertices;// 更新顶点  
                                MeshFilter mf = collider.gameObject.GetComponent<MeshFilter>();
                                mf.mesh = mesh0;
                                collider.sharedMesh = mesh0;
                                collider.gameObject.SetActive(true);
                            }
                            else if (edited == 1)
                            {
                                lstedittype = 3;
                                lstvertices = mesh0.vertices;
                                lsttriangles = mesh0.triangles;

                                // Debug.Log("edited != 0 editface");
                                collider.gameObject.SetActive(false);
                                CombineInstance[] combine = new CombineInstance[2];
                                combine[0].mesh = mesh0;
                                combine[0].transform = Matrix4x4.identity;
                                combine[1].mesh = new Mesh();
                                combine[1].transform = Matrix4x4.identity;
                                PMDLL.updateMeshEx(combine[1].mesh);
                                Mesh newmesh = new Mesh();
                                newmesh.CombineMeshes(combine);

                                collider.sharedMesh = newmesh;
                                MeshFilter mf = lsthit.collider.gameObject.GetComponent<MeshFilter>();
                                mf.mesh = newmesh;
                                collider.gameObject.SetActive(true);
                            }
                        }
                    }
                }
            }
        }
    }
    public void undo()
    {
        Debug.Log("Triangle3D undo");
        if (lstedittype == 3)
        {
            if (lstvertices == null || lsttriangles == null)
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
        else if (lstedittype == 2)
        {
            if (lstvertices == null)
                return;
            MeshFilter mf = gameObject.GetComponent<MeshFilter>();
            gameObject.SetActive(false);
            mf.mesh.vertices = lstvertices;            
            mf.mesh.RecalculateBounds();
            mf.mesh.RecalculateNormals();
            MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
            if (mc) mc.sharedMesh = mf.mesh;
            gameObject.SetActive(true);
        }
        else if (lstedittype == 1)
        {
			gameObject.SetActive(false);
            MeshFilter mf = gameObject.GetComponent<MeshFilter>();
            mf.mesh.colors32 = lstcolor32;
			gameObject.SetActive(true);
        }
    }
    void OnGUI()
    {
		if(!freemod.bfreewalk)
			return;
        Event e = Event.current;
        if (e.isMouse)
        {            
            // pick	
            if (Input.GetMouseButtonDown(0))
            {
                lstmousedownsucc = false;
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                if (getpos(ray, 0))
                {
                    Common.settreated(true, gameObject, 45);
                    lstmousedownsucc = true;
                    ld_mousepos = Input.mousePosition;
                }
               // Debug.Log("GetMouseButtonDown");
            }
            else if (Input.GetMouseButtonUp(0) && lstmousedownsucc)
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                if (((portalbox)portalbox.GetComponent<portalbox>()).PRODUCT.activeSelf && 
                    ((string)((portalbox)portalbox.GetComponent<portalbox>()).selclsnames[0] == "色彩")
                    )
                    createTriangle(ray, 1);
                else
                {
                    createTriangle(ray, 0);
                    portalbox.SendMessage("addtri3doper", gameObject);
                }

                lstmousedownsucc = false;
            }
        }
    }

}
