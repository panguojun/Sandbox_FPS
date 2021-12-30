using UnityEngine;
using System.Collections;
using UnityStandardAssets.CrossPlatformInput;

public class motion : MonoBehaviour 
{
    public GameObject portalbox;
    private float forwardspeed = 1.00F;
    private float sidewardspeed = 0.5F;
	private float turnspeed = 12.5F;
    Vector3 rotcenter;
    // --------------------------------
    // update
    // --------------------------------	
    void Update()
    {      
        {// movement            
            float forward = Input.GetAxis("Vertical") * forwardspeed;
            float sideward = Input.GetAxis("Horizontal") * sidewardspeed;
            forward *= Time.deltaTime;
            sideward *= Time.deltaTime;
			if(forward > 0 || sideward > 0)
            {// 移动
                Vector3 oldpos = transform.position;
                Vector3 movedeta = new Vector3(sideward, 0, forward);
                {// 移动碰撞                    
                    Vector3 npos = oldpos + transform.TransformVector(movedeta);
		            transform.position = npos; 					
                }
            }
			{// 旋转
				transform.RotateAround(rotcenter, Vector3.up, turnspeed * Time.deltaTime);
				transform.LookAt(rotcenter, Vector3.up);
			}			
        }        
    }
	
	public void attach(GameObject go)
	{
		go.transform.position = transform.position;
		go.transform.rotation = transform.rotation;
		go.transform.parent = transform;
		
		portalbox.transform.SetParent(null);
		freemod.bfreewalk = false;	
		Debug.Log("attach");
	}
    void OnGUI()
    {       
        // pick	
        if (Input.GetMouseButtonDown(0))
		{
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
			RaycastHit rhit;
			if (Common.mouseselect(ray, out rhit) && rhit.collider.gameObject == gameObject)
			{
				attach(Camera.main.gameObject);
				Common.settreated(true, gameObject);	
			}		
		}
	}
}
