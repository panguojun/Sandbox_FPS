using UnityEngine;
using System.Collections;

public class PSArt : MonoBehaviour {

    public ParticleSystem PS;
    ParticleSystem.Particle[] ptcs = new ParticleSystem.Particle[58];
    int psCount = 0;
    RaycastHit lsthit;
    bool lstmousedownsucc = false;
    int meshtype = -1;
    // --------------------------------
    // Start
    // --------------------------------
    void Start()
    {
        // 自己创建mesh, 
        if (meshtype == -1)
            return;
        PMDLL.createMesh(this.gameObject, meshtype);
        MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
        if (mc)
            mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;

        // 挂点
        int cnt = PMDLL.gettargetcnt();
        if (cnt > 0)
        {
            Vector3[] targets = new Vector3[cnt];
            PMDLL.getTargets(targets);
            for (int i = 0; i < cnt; i++)
            {
                {
                    //transform.GetChild(i).transform.position = transform.TransformPoint(targets[i]);
                    Vector3 pos = transform.TransformPoint(targets[i]);
                    PS.GetParticles(ptcs);
                    int len = ptcs.Length;
                    ptcs[psCount % len].position = PS.transform.InverseTransformPoint(pos) + PS.transform.InverseTransformDirection(lsthit.normal) * 0.1f;
                    ptcs[psCount % len].startSize = 0.1f;
                    ptcs[psCount % len].startColor = new Color32((byte)Random.Range(0, 255), (byte)Random.Range(0, 255), (byte)Random.Range(0, 255), (byte)Random.Range(200, 255));
                    PS.SetParticles(ptcs, len);
                    psCount++;
                }
            }
        }
    }
    // --------------------------------
    // GUI
    // --------------------------------
    public bool getpos(Ray ray, int type, bool bpriview = false)
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
    public void createPoint(Ray ray, int type, bool bpriview = false)
    {
        if (!lsthit.collider || lsthit.collider.gameObject != gameObject)
            return;
        {
            MeshCollider collider = lsthit.collider as MeshCollider;
            //获取碰撞器所在物体的Mesh网格
            Mesh mesh0 = collider.sharedMesh;
            //获取Mesh网格的所有顶点
            Vector3[] vertices = mesh0.vertices;
            // 传递参数
            //获取mesh的三角形索引，这里的索引的就是模型顶点数组的下标
            int[] triangles = mesh0.triangles;
            int hittriangleIndex = lsthit.triangleIndex * 3;
            if (hittriangleIndex >= 0 && hittriangleIndex < triangles.Length)
            {// 由三角形索引得到方形面     
                Debug.Log("由三角形索引得到方形面" + hittriangleIndex.ToString());
                {// 合法性判定
                    Vector3 p1 = vertices[triangles[hittriangleIndex]]; Debug.Log("p1 = " + p1.ToString());
                    Vector3 p2 = vertices[triangles[hittriangleIndex + 1]]; Debug.Log("p2 = " + p2.ToString());
                    Vector3 p3 = vertices[triangles[hittriangleIndex + 2]]; Debug.Log("p3 = " + p3.ToString());
                    Plane hitplane = new Plane(transform.TransformPoint(p1), transform.TransformPoint(p2), transform.TransformPoint(p3));
                    float enter;
                    if (hitplane.Raycast(ray, out enter))
                    {
                        Vector3 pos = ray.GetPoint(enter);                        
                        PS.GetParticles(ptcs);
                        int len = ptcs.Length;
                        ptcs[psCount % len].position = PS.transform.InverseTransformPoint(pos) + PS.transform.InverseTransformDirection(lsthit.normal) * 0.1f;
                        ptcs[psCount % len].startSize = 0.1f;
                        ptcs[psCount % len].startColor = new Color32((byte)Random.Range(0, 255), (byte)Random.Range(0, 255), (byte)Random.Range(0, 255), (byte)Random.Range(200, 255));
                        PS.SetParticles(ptcs, len);
                        psCount++;
                    }
                }
            }
        }
    }   
    void OnGUI()
    {
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
                    Common.settreated(true, gameObject);
                    lstmousedownsucc = true;
                }
            }
            if (Input.GetMouseButtonUp(0) && lstmousedownsucc)
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                createPoint(ray, 0);
            }
        }
    }
}