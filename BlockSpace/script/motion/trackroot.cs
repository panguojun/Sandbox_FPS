using UnityEngine;
using System.Collections;

public class trackroot : MonoBehaviour
{
    public GameObject TRACKOBJ;
    Transform currentTransform;
    ArrayList trackList = new ArrayList();
    int currentPos = -1;
    // Use this for initialization
    public class Track_Unit_t
    {
        public Vector3 position;
        public Quaternion rotation;
        public int type;

        public Track_Unit_t(Vector3 pos, Quaternion qua, int tracktype = 0)
        {
            position = pos;
            rotation = qua;
            type = tracktype;
        }
    };
    // --------------------------------
    Vector3 getTrackNextPos()
    {
        if (trackList.Count > 0)
            return ((Track_Unit_t)trackList[(currentPos + 1) % trackList.Count]).position;
        else
            return Vector3.zero;
    }
    Quaternion getTrackNextQua()
    {
        if (trackList.Count > 0)
            return ((Track_Unit_t)trackList[(currentPos + 1) % trackList.Count]).rotation;
        else
            return Quaternion.identity;
    }
    void Start()
    {

    }
    // --------------------------------
    void Update()
    {
        if (freemod.bfreewalk || currentTransform == null)
        {
            currentPos = -1;
            return;
        }

        if (currentPos >= 0)
        {
            Vector3 np = getTrackNextPos();
            if (Camera.main.transform.parent == null)
                np += Vector3.up * 1.7f;
            if ((currentTransform.position - np).magnitude < 1 && currentPos < trackList.Count - 1)
            {
                currentPos = (currentPos + 1) % trackList.Count;
            }
            Debug.Log("Update " + currentPos.ToString());
            currentTransform.position = Vector3.Lerp(currentTransform.position, np, 0.01f);
            if(Camera.main.transform.parent)
            {
                Quaternion nq = getTrackNextQua();
                currentTransform.rotation = Quaternion.Lerp(currentTransform.rotation, nq, 0.01f);
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
                if (freemod.bfreewalk)
                {
                    RaycastHit hit;
                    Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                    if (Common.mouseselect(ray, out hit) && 
                        (hit.collider.gameObject == gameObject || hit.collider.transform.parent && hit.collider.transform.parent.gameObject == gameObject)
                        )
                    {// 编辑
                        Common.settreated(true, gameObject);

                        {// 添加轨道
                            GameObject bd = Instantiate(TRACKOBJ);
                            bd.name = "TRACKOBJ" + bd.GetInstanceID().ToString();
                            bd.SetActive(true);
                            bd.name = "hit.normal" + hit.normal.ToString();
                            Vector3 dir = hit.normal;dir.y = 0;
                            bd.transform.position = hit.collider.transform.position + dir.normalized * 2;
                            bd.transform.rotation = hit.collider.transform.rotation;
                            bd.transform.parent = transform;

                            Track_Unit_t tu = new Track_Unit_t(bd.transform.position, bd.transform.rotation);
                            trackList.Add(tu);
                        }
                    }
                }
                else
                {// 上车
                    RaycastHit hit;
                    Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                    if (Common.mouseselect(ray, out hit) &&
                        (hit.collider.gameObject == gameObject || hit.collider.transform.parent && hit.collider.transform.parent.gameObject == gameObject)
                        )
                    {
                        currentPos = 0;                        
                        if(Camera.main.transform.parent && Camera.main.transform.parent.name.Contains("Building"))
                        {
                            Camera.main.transform.parent.position = transform.position + Vector3.up * 1;
                            currentTransform = Camera.main.transform.parent;
                        }
                        else
                        {
                            Camera.main.transform.position = transform.position + Vector3.up * 1;
                            currentTransform = Camera.main.transform;
                        }
                    }
                }
            }
        }
    }
}
