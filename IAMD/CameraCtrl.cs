using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraCtrl : MonoBehaviour {
    public GameObject sceneroot;
	public float mindis, maxdis;
    public float MouseWheelSensitivity = 1.0f;
	private float wheelspeed = 0;
	private float rotspeedx = 0;
    private float rotspeedy = 0;
	private float camexpecteddis = 1;
	
    void Start()
    {
        transform.LookAt(sceneroot.transform.position);
		camexpecteddis = (transform.position-sceneroot.transform.position).magnitude;
    }
	// Update is called once per frame
	void Update () {
        if (!GUI.changed)
        {
            {
                float x = Input.GetAxis("Mouse X");
                float y = -Input.GetAxis("Mouse Y");
				if((Input.GetMouseButton(1)) && x != 0)
				{
					rotspeedx = x;
					transform.RotateAround(sceneroot.transform.position, Vector3.up, x * 5.0f);
				}
				else
				{
					transform.RotateAround(sceneroot.transform.position, Vector3.up, rotspeedx * 5.0f);
					rotspeedx = Mathf.Lerp(rotspeedx, 0, 0.01f);
				}
				if ((Input.GetMouseButton(1)) && y != 0)
				{

					if ((transform.forward.y < 0.8 || y > 0) && (transform.forward.y > -0.8 || y < 0))
					{
						rotspeedy = y;
						transform.RotateAround(sceneroot.transform.position, transform.right, y * 2.0f);
					}
				}
				else
				{
					if ((transform.forward.y < 0.8 || rotspeedy > 0) && (transform.forward.y > -0.8 || rotspeedy < 0))
					{
						transform.RotateAround(sceneroot.transform.position, transform.right, rotspeedy * 2.0f);
					}
					rotspeedy = Mathf.Lerp(rotspeedy, 0, 0.01f);
				}
            }
            
			// scale
			float wheel = Input.GetAxis("Mouse ScrollWheel");
			{
				if(wheel != 0)
				{
					float tMouseWheelSensitivity = wheel > 0 ?
						MouseWheelSensitivity * Mathf.Clamp(transform.position.magnitude / 5, 0.1f, 10) :
						MouseWheelSensitivity * Mathf.Clamp(transform.position.magnitude / 5, 1, 10);

					Vector3 npos = transform.position + transform.TransformDirection(Vector3.forward) * (wheel * tMouseWheelSensitivity);
					if ((Vector3.Dot(sceneroot.transform.position-npos, transform.forward) > 20) && ((npos-sceneroot.transform.position).magnitude < 11600))
					{
						transform.position = npos;
						camexpecteddis = (transform.position-sceneroot.transform.position).magnitude;
						wheelspeed = wheel;
					}
					else
						wheelspeed = 0;

				}
				else
				{
					float tMouseWheelSensitivity = wheelspeed > 0 ?
						MouseWheelSensitivity * Mathf.Clamp(transform.position.magnitude / 2, 0.1f, 10) :
						MouseWheelSensitivity * Mathf.Clamp(transform.position.magnitude / 2, 1, 10);

					Vector3 npos = transform.position + transform.TransformDirection(Vector3.forward) * (wheelspeed * tMouseWheelSensitivity);
					if ((Vector3.Dot(sceneroot.transform.position-npos, transform.forward) > 20) && ((npos-sceneroot.transform.position).magnitude < 11600))
					{
						//camexpecteddis = (transform.position-sceneroot.transform.position).magnitude;
						transform.position = npos;
						wheelspeed = Mathf.Lerp(wheelspeed, 0, 0.1f);
					}
					else
						wheelspeed = 0;
				}
				
				if(camexpecteddis < 20)
					camexpecteddis = 20;
				
				Vector3 dir = transform.position - sceneroot.transform.position;dir.Normalize();
				Vector3 target = sceneroot.transform.position + dir * camexpecteddis;
				Vector3 npos2 = Vector3.Lerp(transform.position, target, 0.1f);
				float dis = (npos2-sceneroot.transform.position).magnitude;	
				if(dis > 20 && dis > camexpecteddis)
				{
					transform.position = npos2;
				}
			}
			
            // move the height of cam
            if (Input.GetMouseButton(2))
            {
                float y = -Input.GetAxisRaw("Mouse Y");
				transform.Translate(Vector3.up * (y * Mathf.Abs((transform.position-sceneroot.transform.position).magnitude) / 30));
            }
			
			transform.LookAt(sceneroot.transform.position);
        }
	}
}