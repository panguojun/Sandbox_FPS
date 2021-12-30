using UnityEngine;
using System.Collections;

public class Growing : MonoBehaviour
{
    public GameObject PORTALBOX;
    public GameObject PACKCUBE;
    public GameObject PLANTMGR;
    public int growingtype = 0; // 0 代表自动生长 1 代表外部驱动生长
    float autogtime = 0;
    float lgtime = 0;
    // Use this for initialization
    void Start()
    {
        transform.GetChild(0).gameObject.SetActive(false);
    }
    // Update is called once per frame
    void Update()
    {
        if (growingtype == 1)
            return;

        autogtime += Time.deltaTime;
        if (autogtime - lgtime >= 10)
        {// 每秒钟更新
            lgtime = autogtime;
            growing(autogtime);
        }
    }
    public void growing(float time)
    {
        if (transform.GetChild(0).gameObject.activeSelf == true)
        {// 已经成熟了
            return;
        }
        // 生长        
        if (time > 100)
        {
            time = 100;
            PMDLL.growing_plant(gameObject, 0, Vector3.zero, Vector3.up, 0.01f, time / 100);
            // 挂点
            int cnt = PMDLL.gettargetcnt();
            if (cnt > 0)
            {
                Vector3[] targets = new Vector3[cnt];
                PMDLL.getTargets(targets);
                for (int i = 0; i < cnt; i++)
                {
                    if (transform.GetChild(i))
                    {
                        transform.GetChild(i).gameObject.SetActive(true);
                        transform.GetChild(i).transform.position = transform.TransformPoint(targets[i]);
                    }
                }
            }
        }
        else
        {
            PMDLL.growing_plant(gameObject, 0, Vector3.zero, Vector3.up, 0.01f, time / 100);
        }
    }

    // 采摘果实
    void OnGUI()
    {
        return;
        Event e = Event.current;
        if (e.isMouse)
        {
            // pick	
            if (Input.GetMouseButtonDown(0))
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Common.mouseselect(ray, out hit) && (hit.collider.gameObject == transform.GetChild(0).gameObject))
                {
                    //PORTALBOX.SendMessage("pickedupproduct", transform.GetChild(0).gameObject);    
                    PACKCUBE.SendMessage("addmine", transform.GetChild(0).gameObject.name); // 植物产出矿物而不是材料
                    transform.GetChild(0).gameObject.SetActive(false);
                    PLANTMGR.SendMessage("picked", gameObject);
                }
            }
        }
    }
}
