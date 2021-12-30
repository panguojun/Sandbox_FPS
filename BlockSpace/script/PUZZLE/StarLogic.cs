using UnityEngine;
using System.Collections;

public class StarLogic : MonoBehaviour
{
    public GameObject portalbox;
    Vector3 triggerPos;
    // Use this for initialization
    void Start()
    {
        triggerPos = gameObject.transform.position;
        ((portalbox)portalbox.GetComponent<portalbox>()).byPuzzle = true;
    }

    // Update is called once per frame
    void Update()
    {
        if (portalbox.activeSelf)
            return;
        if((Camera.main.transform.position - triggerPos).magnitude < 1)
        {// 触发            
            portalbox.SetActive(true);
            portalbox.transform.position = triggerPos;
        }
    }
    void OnGUI()
    {
        if (!freemod.bfreewalk)
            return;
        Event e = Event.current;
        if (e.isMouse)
        {
            //// pick	
            //if (Input.GetMouseButtonDown(0))
            //{
               
            //    Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);                
            //    {
            //        Common.settreated(true, gameObject);
                    
            //    }
                
            //}
        }
    }
}
