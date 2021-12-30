using UnityEngine;
using System.Collections;
using UnityStandardAssets.CrossPlatformInput;

public class robot : MonoBehaviour 
{
    public GameObject head;
    public GameObject lhand;
    public GameObject rhand;
    public GameObject lfeet;
    public GameObject rfeet;
    float size = 0.1f;
    int currentPos = 0;
    int state = 1; // 0 dead 1 alive 
    // --------------------------------
    // update
    // --------------------------------	
    Vector3 getTrackNextPos()
    {
        if (freemod.footsteps.Count > 0)
            return ((Vector3)freemod.footsteps[(currentPos + 1) % freemod.footsteps.Count]);
        else
            return Vector3.zero;
    }
    void Update()
    {
        if (state == 1)
        {
            if (currentPos >= 0 && freemod.footsteps.Count > 0)
            {
                Vector3 np = getTrackNextPos();
                if ((transform.position - np).magnitude < 1)
                {
                    currentPos = (currentPos + 1) % freemod.footsteps.Count;
                }
                transform.position = Vector3.Lerp(transform.position, np, 0.01f);
            }
            // animation
            if (head) head.transform.position += transform.TransformDirection(Vector3.left) * (size * 0.001f * Mathf.Sin(Time.time * 3));
            if (lhand) lhand.transform.position += transform.TransformDirection(Vector3.forward) * (size * 0.01f * Mathf.Sin(Time.time * 4));
            if (rhand) rhand.transform.position += transform.TransformDirection(Vector3.forward) * (size * 0.01f * Mathf.Sin(Time.time * 4 + Mathf.PI));
            if (lfeet) lfeet.transform.position += transform.TransformDirection(Vector3.forward) * (size * 0.01f * Mathf.Sin(Time.time * 4 + Mathf.PI));
            if (rfeet) rfeet.transform.position += transform.TransformDirection(Vector3.forward) * (size * 0.01f * Mathf.Sin(Time.time * 4));
        }
    }
    void OnGUI()
    {
        Event e = Event.current;
        if (e.isMouse && Input.GetMouseButtonDown(0))
        {
            if (!freemod.bfreewalk)
            {
                RaycastHit hit;
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                if (Common.mouseselect(ray, out hit) &&
                    (hit.collider.gameObject == gameObject || hit.collider.transform.parent && hit.collider.transform.parent.gameObject == gameObject)
                    )
                {// 编辑
                    Common.settreated(true, gameObject);
                    if (state == 1)
                        state = 0;
                    else
                        state = 1;

                    Debug.Log("hit robot " + state.ToString() + ", " + gameObject.name);
                }
            }
        }
    }
    void Start()
    {
        if (head) head.transform.position = transform.position + transform.up * size;
        if(lhand) lhand.transform.position = transform.position + transform.TransformDirection(Vector3.left) * size;
        if(rhand) rhand.transform.position = transform.position + transform.TransformDirection(Vector3.right) * size;
        if(lfeet) lfeet.transform.position = transform.position - transform.up * (2 * size) + transform.TransformDirection(Vector3.left) * size;
        if(rfeet) rfeet.transform.position = transform.position - transform.up * (2 * size) + transform.TransformDirection(Vector3.right) * size;       
    }
}
