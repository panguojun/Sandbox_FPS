using UnityEngine;
using System.Collections;

public class RoverCtrl : MonoBehaviour {
    public GameObject ROVER;

    public GameObject LASER;
    public GameObject HALO;
    public GameObject POINTER;    
    // ---------------------------------------------------
    // Use this for initialization
    void Start ()
    {
        float laserlen = 1;
        Vector3 pos1 = Vector3.zero;
        Vector3 pos2 = pos1 + Vector3.up * laserlen;
        ((Line3d)LASER.GetComponent<Line3d>()).setLine(pos1, pos2);
    }	
	// Update is called once per frame
	void Update () {
        // 自动返回原来方向
        transform.rotation = Quaternion.Lerp(transform.rotation, transform.parent.rotation, 0.025f);
    }
    // --------------------------------------------------
    // 轮盘的控制
    // --------------------------------------------------
    bool castAnchor = false;
    Vector2 anchorpos;
    float cross2d(Vector2 a, Vector2 b)
    {
        return (a.x * b.y - a.y * b.x);
    }
    void OnGUI()
    {
        Event e = Event.current;
        if (e.isMouse)
        {
            // pick	
            if (Input.GetMouseButtonDown(0))
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Common.mouseselect(ray, out hit))
                {
                    if (hit.collider.gameObject == HALO)
                    {
                        anchorpos = new Vector2(Input.mousePosition.x, Input.mousePosition.y);
                        castAnchor = true;
                        Common.settreated(true, gameObject);
                    }
                    else if (hit.collider.gameObject == POINTER)
                    {
                        ROVER.SendMessage("moveit");
                        Common.settreated(true, gameObject);
                    }                              
                }
            }
            else if (Input.GetMouseButtonUp(0))
            {
               // if (!Common.treated)
                {
                    castAnchor = false;
                }
            }
        }
        if (e.type == EventType.MouseDrag && e.button == 0)
        {           
            if (castAnchor)
            {
                Vector2 o = Camera.main.WorldToScreenPoint(HALO.transform.position); 
                Vector2 om = new Vector2(Input.mousePosition.x, Input.mousePosition.y) - o; 
                Vector2 oa = anchorpos - o; 

                float dang = Vector2.Angle(oa, om);
                float deta = cross2d(om, oa);
                Transform nt = transform;
                nt.Rotate(Vector3.up, deta > 0 ? dang : -dang, Space.Self);
                transform.rotation = Quaternion.Lerp(transform.rotation, nt.rotation, 0.01f);

                ROVER.SendMessage("turn", deta);

                anchorpos = new Vector2(Input.mousePosition.x, Input.mousePosition.y);

                Common.settreated(true, gameObject);
            }                      
        }
    }
}