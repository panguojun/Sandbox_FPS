using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections.Generic;
public class Skeleton : MonoBehaviour {
    public GameObject BONENODE;
    public GameObject Body;
    public GameObject[] BODYPARTS;
    public GameObject GROUND;
	public GUIStyle buttonstyle;
	
    GameObject rootbone;
    GameObject spinebody;
    GameObject selectbone;

    const float c_size = 1.25f;
    const int bodybones = 4;
    const int tailbones = 8;

    float gtime;
    int numbones = 0;
    bool bskinvis = true;
    bool bPlaying = false;
    Vector3 oldpos;
    Vector3 movingvir;
    float movetime = 0;
    int currentPart = -1;
    Vector3 movingtarget;

    // -------------------------------------------------------------------------------------
    void Start () {
        setup();
        gtime = Time.time;
    }
    // -------------------------------------------------------------------------------------
    // 动画
    // -------------------------------------------------------------------------------------
    // idle
    float vecangle(Vector3 v1, Vector3 v2)
    {
        float ang;
        v1.y = 0;v2.y = 0;
        float dot = Mathf.Clamp(Vector3.Dot(v1, v2), -1, 1);
        if (Vector3.Cross(v1, v2).y > 0)
        {
            ang = Mathf.Acos(dot);
        }
        else
        {
            ang = -Mathf.Acos(dot);
        }
        return ang;
    }
    void Update()
    {
        //if(bskinvis)
        {// 脊椎动画
            if (!rootbone)
                return;
            if (!animationstopped)
            { // position
                Vector3 deta = (movingtarget - transform.position);
                if (deta.magnitude > 1)
                {
                    movingvir = deta.normalized;
                    transform.position += transform.forward * (Time.deltaTime * 1.5f);
                }
            }
            // rotation
            GameObject fixedbone = rootbone;
            if (movingvir.magnitude > 0.1f)
            {
                float timealpha = (Time.time - movetime) / 100.5f;                
                float dang0 = vecangle(transform.forward, movingvir);                
                transform.Rotate(0, dang0 * 4, 0);
                {// 更新骨骼
                    float ldang = dang0;
                    GameObject b = rootbone;
                    int i = 0;
                    while (b.transform.childCount > 0)
                    {
                        GameObject cb = b.transform.GetChild(0).gameObject; // 骨骼是第一个挂点                    
                        {// 脊椎骨之间的变化
                            float dang = (vecangle(b.transform.forward, movingvir)) * Mathf.Abs(Mathf.Cos((float)i / numbones * Mathf.PI / 2 + timealpha * Mathf.PI / 2));
                            b.transform.Rotate(0, (dang - ldang) * 4, 0);
                            ldang = dang;
                        }
                        b = cb;
                        if (i == numbones - 1)
                            fixedbone = b;
                        i++;
                    }
                }
            }
            if (oldpos != Vector3.zero)
                transform.position -= (transform.InverseTransformPoint(fixedbone.transform.position) - oldpos);
            oldpos = transform.InverseTransformPoint(fixedbone.transform.position);
        }
    }
    // 站起来
    IEnumerator stand(float alpha)
    {
        yield return new WaitForSeconds(1 / 30.0f);
        {// 脊椎动画
            float dang = Mathf.Sin(alpha * Mathf.PI) * Mathf.PI;
            GameObject b = rootbone;
            int i = 0;
            while (b.transform.childCount > 0)
            {
                GameObject cb = b.transform.GetChild(0).gameObject; // 骨骼是第一个挂点 
                if ((i++) < 4)
                {// 脊椎骨之间的变化
                    cb.transform.Rotate(dang, 0, 0);
                }
                b = cb;
            }
        }
        if (alpha < 1)
        {
            StartCoroutine(stand(alpha + 0.1f));
        }
    }
    // 前进
    bool animationstopped = false;
    IEnumerator moveforward(float alpha)
    {
        yield return new WaitForSeconds(1 / 60.0f);        
        // move
        Vector3 deta = (movingtarget - transform.position);
        movingvir = deta.normalized;
        transform.position += transform.forward * (Time.deltaTime * 1.1f);
        if (!animationstopped && alpha < 1 && deta.magnitude > 0.25)
        {
            StartCoroutine(moveforward(alpha + 0.01f));
        }       
    }
    // 坐下
    IEnumerator sitdown(float alpha)
    {
        yield return new WaitForSeconds(1 / 30.0f);
        {// 脊椎动画
            float dang = Mathf.Sin(alpha * Mathf.PI) * Mathf.PI / 4;
            GameObject b = rootbone;
            int i = 0;
            while (b.transform.childCount > 0)
            {
                GameObject cb = b.transform.GetChild(0).gameObject; // 骨骼是第一个挂点 
                if ((i++) < 4)
                {// 脊椎骨之间的变化
                    cb.transform.Rotate(-dang, 0, 0);
                }
                b = cb;
            }
        }
        if (alpha < 1)
        {
            StartCoroutine(sitdown(alpha + 0.1f / 4));
        }
    }

    // -------------------------------------------------------------------------------------
    GameObject addbone(GameObject lnb, GameObject template = null)
    {
        if (template == null)
            template = BONENODE;
        GameObject nb = GameObject.Instantiate(template);

        if (lnb == gameObject)
            rootbone = nb;
        nb.SetActive(true);
        nb.transform.parent = lnb.transform;
        nb.transform.position = lnb.transform.position;
        nb.transform.rotation = lnb.transform.rotation;
        numbones++;
        return nb;
    }
    GameObject addheadbone(GameObject lnb, GameObject template = null)
    {
        if (template == null)
            template = BONENODE;
        GameObject nb = GameObject.Instantiate(template);

        rootbone = nb;
        nb.SetActive(true);

        GameObject parent = lnb.transform.parent.gameObject;
        nb.transform.parent = parent.transform;
        lnb.transform.parent = nb.transform;

        nb.transform.position = lnb.transform.position;
        lnb.transform.position = nb.transform.position - lnb.transform.forward * (c_size * lnb.transform.lossyScale.z);
        nb.transform.rotation = lnb.transform.rotation;
        numbones++;
        return nb;
    }
    void setup()
    {
        {// 脊椎
            int num = bodybones + tailbones;
            GameObject lnb = gameObject;
            for (int i = 0; i < num; i++)
            {
                GameObject nb = addbone(lnb);
                if(i > 0)
                {// 脊椎骨之间的变化
                    if (i < bodybones)
                    {// 上半身
                        nb.transform.Rotate(-5f, 0, 0);
                        nb.transform.position += (-lnb.transform.forward * (c_size * lnb.transform.lossyScale.z));
                    }
                    else
                    {// 下半身
                        nb.transform.Rotate(10, 0, 0);
                        nb.transform.position += (-lnb.transform.forward * (c_size * lnb.transform.lossyScale.z));
                    }                   
                    float t = ((float)i / num);
                    nb.transform.localScale *= Mathf.Lerp(0.5f, 2, t < 0.5f ? t / 0.5f : (1 - t) / 0.5f);
                }
                lnb = nb;
            }
        }
        {// body
            spinebody = GameObject.Instantiate(Body);
            spinebody.transform.SetParent(gameObject.transform);
            spinebody.SetActive(true);
            spinebody.SendMessage("setSpineRoot", rootbone);           
        }
    }
    void onskinvis(bool vis)
    {
        bskinvis = vis;
    }
	static bool mouseselectex(Ray ray, out RaycastHit rhit, float maxDis = 100.0f)
    {       
        bool treated = false;
        int layerMask = 1 << 8; // 不包括
        if (Physics.Raycast(ray, out rhit, maxDis, ~layerMask))
        {           
            return true;
        }
        return false;
    }
	static bool mouseselect(Ray ray, out RaycastHit rhit, float maxDis = 100.0f)
    {
        int layerMask = 1 << 8; // 不包括
        if (Physics.Raycast(ray, out rhit, maxDis, ~layerMask))
        //    if (Physics.Raycast(ray, out rhit, maxDis))
        {
            //  Debug.Log("mouseselect Physics.Raycast " + rhit.collider.gameObject.ToString()); 
          
            return true;
        }
        return false;
    }
    // -------------------------------------------------------------------------------------    
    void OnGUI()
    {
        bool bhit = false;        
        Event e = Event.current;
        if (e.isMouse)
        {
            if (Input.GetMouseButtonDown(0))
            {
                RaycastHit hit;
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                if (bhit = mouseselectex(ray, out hit) && hit.collider.gameObject.CompareTag("spine"))
                {
                    GameObject lnb = hit.collider.gameObject;
                    Vector3 pos = hit.point;
                    Vector3 dir = (pos - lnb.transform.position).normalized;
                    float dotdir = Vector3.Dot(hit.normal, lnb.transform.forward);
                    if ((lnb.transform.childCount == 0 && dotdir < -0.01f)
                        || dotdir > 0.01f && (!lnb.transform.parent.CompareTag("spine"))
                        )
                    {// 添加脊椎骨                        
                        if (dotdir < -0.01f)
                        {
                            float dot = Vector3.Dot(dir, lnb.transform.up);
                            GameObject nb = addbone(lnb);
                            nb.tag = "spine";
                            nb.transform.Rotate(30 * dot, 0, 0);
                            nb.transform.localScale = lnb.transform.localScale;
                            nb.transform.position += (-lnb.transform.forward * (c_size * lnb.transform.lossyScale.z));
                            nb.GetComponent<MeshRenderer>().enabled = true;
                        }
                        else if (dotdir > 0.01f)
                        {
                            GameObject nb = addheadbone(lnb);
                            nb.tag = "spine";
                            nb.GetComponent<MeshRenderer>().enabled = true;
                            spinebody.SendMessage("setSpineRoot", rootbone);
                        }
                    }
                    else if (currentPart >= 0)
                    {// 添加部件
                        float dot = Vector3.Dot(hit.normal, lnb.transform.right);
                        if (Mathf.Abs(dot) > 0.8f)
                        {
                            if (currentPart >= 4)
                            {// 头部
                                selectbone = lnb;

                                GameObject LHand = GameObject.Instantiate(BODYPARTS[currentPart]);
                                LHand.name = BODYPARTS[currentPart].name;
                                LHand.SetActive(true);
                                LHand.transform.SetParent(selectbone.transform);

                                LHand.transform.position = selectbone.transform.position;// + selectbone.transform.up * (selectbone.transform.lossyScale.y);
                                LHand.transform.SetParent(selectbone.transform);
                                LHand.transform.rotation = selectbone.transform.rotation;
                                LHand.transform.Rotate(0, 0, -90);
                                //LHand.transform.localScale = selectbone.transform.localScale * 50;
                                //LHand.transform.localScale = Vector3.one * 2;
                            }
                            else if (currentPart >= 0)
                            {// 四肢
                                selectbone = lnb;

                                GameObject LHand = GameObject.Instantiate(BODYPARTS[currentPart]);
                                LHand.name = BODYPARTS[currentPart].name;
                                LHand.SetActive(true);
                                LHand.transform.SetParent(selectbone.transform);
                                LHand.transform.position = selectbone.transform.position - selectbone.transform.right * (selectbone.transform.lossyScale.x);      
                                LHand.transform.localScale = selectbone.transform.localScale * 20;
                                LHand.transform.rotation = selectbone.transform.rotation;

                                GameObject RHand = GameObject.Instantiate(BODYPARTS[currentPart + 1]);
                                RHand.name = BODYPARTS[currentPart + 1].name;
                                RHand.SetActive(true);
                                RHand.transform.SetParent(selectbone.transform);
                                RHand.transform.position = selectbone.transform.position + selectbone.transform.right * (selectbone.transform.lossyScale.x);
                                RHand.transform.localScale = selectbone.transform.localScale * 20;
                                RHand.transform.rotation = selectbone.transform.rotation;
                            }
                        }
                    }
                    else if (Vector3.Dot(hit.normal, lnb.transform.forward) > 0.01f)
                    {// 删除骨骼
                        float dot = Vector3.Dot(dir, lnb.transform.forward);
                        if (dot > 0)
                        {
                            numbones--;
                            Destroy(lnb); 
                        }
                    }
                    else
                    {
                        if (Vector3.Dot(hit.normal, lnb.transform.up) > 0.01f)
                        {// 放大
                            lnb.transform.localScale *= 1.1f;
                            if (lnb.transform.childCount > 0)
                            {
                                GameObject cb = lnb.transform.GetChild(0).gameObject;
                                if (cb && cb.CompareTag("spine"))
                                {
                                    cb.transform.localScale /= 1.1f;
                                    cb.transform.position = lnb.transform.position + (-lnb.transform.forward * (c_size * lnb.transform.lossyScale.z));
                                }
                            }
                        }
                        if (Vector3.Dot(hit.normal, lnb.transform.up) < -0.01f)
                        {// 缩小
                            lnb.transform.localScale *= 0.9f;
                            if (lnb.transform.childCount > 0)
                            {
                                GameObject cb = lnb.transform.GetChild(0).gameObject;
                                if (cb && cb.CompareTag("spine"))
                                {
                                    cb.transform.localScale *= 1.1f;
                                    cb.transform.position = lnb.transform.position + (-lnb.transform.forward * (c_size * lnb.transform.lossyScale.z));
                                }
                            }
                        }
                    }
                }
            }
        }
        {// GUI
            // model
            int uiposy = 128;            
            // play
            if (GUI.Button(new Rect(Screen.width - 128, uiposy, 64, 64), "play"))
            {
                animationstopped = false;
                GROUND.transform.position = Vector3.zero - Vector3.up * 1;
                bPlaying = true;
               // if (selectbone)
                {
                    int ind = 0;
                    GameObject b = rootbone;
                    while (b)
                    {
                        int childcnt = b.transform.childCount;
                        if (childcnt > 1)
                        {
                            for (int i = 1; i < childcnt; i++)
                            {
                                GameObject bb = b.transform.GetChild(i).gameObject;
                                Animator animation = bb.GetComponent<Animator>();
                                animation.Play(bb.gameObject.name);
                            }
                        }
                        ind++;
                        if (childcnt > 0)
                            b = b.transform.GetChild(0).gameObject;
                        else
                            break;
                    }
                }
                return;
            }
            // edit
            uiposy += 64 + 20;

            if (GUI.Button(new Rect(Screen.width - 128, uiposy, 64, 64), "edit", buttonstyle))
            {
                animationstopped = true;
                GROUND.transform.position = Vector3.zero - Vector3.up * 4;
                transform.position = Vector3.zero;
                bPlaying = false;
                return;
            }
        }
        int oldcurrentPart = currentPart;
        {// 部件库
            int uiposy = 64; 
            if (GUI.Button(new Rect(64, uiposy, 64 * (currentPart == 0 ? 1.2f : 1), 64 * (currentPart == 0 ? 1.2f : 1)), "leg"))
            {
                currentPart = 0;
                return;
            }
            uiposy += 64 + 20;
            if (GUI.Button(new Rect(64, uiposy, 64 * (currentPart == 2 ? 1.2f : 1), 64 * (currentPart == 2 ? 1.2f : 1)), "hand"))
            {
                currentPart = 2;
                return;
            }
            uiposy += 64 + 20;
            if (GUI.Button(new Rect(64, uiposy, 64 * (currentPart == 4 ? 1.2f : 1), 64 * (currentPart == 4 ? 1.2f : 1)), "head"))
            {
                currentPart = 4;
                return;
            }
        }
        if (e.isMouse)
        {
            if (Input.GetMouseButtonUp(0) && !bhit && oldcurrentPart == currentPart)
            {
                currentPart = -1;
            }            
        }
        if (Input.GetMouseButtonDown(0))
        {
            RaycastHit hit;
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            if (bPlaying && (mouseselectex(ray, out hit)) && hit.collider.gameObject.CompareTag("ground"))
            {
                Vector3 target = hit.point; target.y = 0;
                movingtarget = target;
                movetime = Time.time;
                //StartCoroutine(moveforward(0));
            }
        }
    }
}