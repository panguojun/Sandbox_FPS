using UnityEngine;
using System.Collections;
using UnityStandardAssets.CrossPlatformInput;

public class freemod : MonoBehaviour 
{
    static public bool movementlocked = false;
    static public bool bfreewalk = true;
    private float forwardspeed = 1.00F;
    private float sidewardspeed = 0.5F;
	private float turnspeed = 2.5F;
    float MouseWheelSensitivity = 2.5f;
    Vector3 lstfixgroundpos;
    Vector3 rotcenter;
	private float wheelspeed = 0;
	static public float rotspeedx = 0.0f;
    static public float rotspeedy = 0.0f;

    static public ArrayList footsteps = new ArrayList();
    // --------------------------------
    // update
    // --------------------------------	
    void Update()
    {
        if (!movementlocked)
        {
            // movement
            float forward = Input.GetAxis("Vertical") * (bfreewalk ? 2 : 1) * forwardspeed;
            float sideward = Input.GetAxis("Horizontal") * (bfreewalk ? 2 : 1) * sidewardspeed;
            forward *= Time.deltaTime;
            sideward *= Time.deltaTime;

            {// 移动
                Vector3 oldpos = transform.position;
                Vector3 movedeta = new Vector3(sideward, 0, forward);
                {// 移动碰撞                    
                    Vector3 npos = oldpos + transform.TransformVector(movedeta);
                    Ray moveray = new Ray(oldpos, (npos - oldpos).normalized);
                    if (!Common.rayIsHit(moveray, movedeta.magnitude + 0.5f))
                    {
                        transform.position = npos;
                    }
                }
                if (!bfreewalk)
                {// 调整高度
                    bool bGround = false;
                    if ((transform.position - lstfixgroundpos).magnitude > 0.1f)
                    {
                        Vector3 groundpos = GeoStaticData.getspherepos(transform.position, -8.0f);
                        {// 跟建筑物的碰撞
                            Vector3 deta = (groundpos - transform.position);
                            Ray moveray = new Ray(transform.position, deta.normalized);
                            RaycastHit hit;
                            if (Common.rayHitTest(moveray, out hit, 18))
                            {
                                groundpos = hit.point - deta.normalized * 1.0f;
                                lstfixgroundpos = groundpos;
                                bGround = true;
                                if(hit.collider.gameObject.name.Contains("Building"))
                                    Camera.main.transform.parent = hit.collider.gameObject.transform;
                            }
                        }
                    }
                    else
                    {
                        bGround = true;
                    }
                    if (bGround)
                        transform.position = Vector3.Slerp(transform.position, lstfixgroundpos, 0.05f);
                    else
                    {
                        transform.position = oldpos;
                    }
                    if(footsteps.Count == 0 || (((Vector3)footsteps[footsteps.Count - 1]) - transform.position).magnitude > 0.25)
                    {
                        footsteps.Add(transform.position);
                        Debug.Log(transform.position.ToString() + ", ");
                    }                    
                }
            }
        }
        if (bfreewalk)
        {
            // 旋转中心
            if (Input.GetMouseButtonDown(1))
            {
                Ray ray = new Ray(transform.position, transform.forward);
                rotcenter = Vector3.zero;//transform.position;// ray.GetPoint(50.0f);
                RaycastHit hit;
                if (Common.mouseselect(ray, out hit, 38.0f))
                {
                    if(!hit.collider.gameObject.name.Contains("MOON"))
                        rotcenter = hit.point;
                }
            }
            if (Input.GetMouseButton(2))
            {// 平移
                float x = -Input.GetAxis("Mouse X");
                float y = -Input.GetAxis("Mouse Y");

                float dis = (transform.position - rotcenter).magnitude;
                float deta = Mathf.Clamp(dis / 38.0f, 0.1f, 0.5f);
                Vector3 movedeta = new Vector3((x * deta), (y * deta), 0);
                transform.Translate(movedeta);
            }
            // rotation
			{
                float x = Input.GetAxis("Mouse X");
                float y = -Input.GetAxis("Mouse Y");
				if((Input.GetMouseButton(1)) && x != 0)
				{
					rotspeedx = 1.0f * x;
					transform.RotateAround(rotcenter, Vector3.up, x * 1.0f);
				}
				else
				{
					transform.RotateAround(rotcenter, Vector3.up, rotspeedx * 4.0f);
					rotspeedx = Mathf.Lerp(rotspeedx, 0, 0.1f);
				}
				if ((Input.GetMouseButton(1)) && y != 0)
				{

					if ((transform.forward.y < 0.8 || y > 0) && (transform.forward.y > -0.8 || y < 0))
					{
						rotspeedy = y;
						transform.RotateAround(rotcenter, transform.right, y * 1.0f);
					}
				}
				else
				{
					if ((transform.forward.y < 0.8 || rotspeedy > 0) && (transform.forward.y > -0.8 || rotspeedy < 0))
					{
						transform.RotateAround(rotcenter, transform.right, rotspeedy * 1.0f);
					}
					rotspeedy = Mathf.Lerp(rotspeedy, 0, 0.1f);
				}
            }
            // scale
            float wheel = Input.GetAxis("Mouse ScrollWheel");
            if (bfreewalk)
            {
                if(wheel != 0)
				{
					float tMouseWheelSensitivity = wheel > 0 ?
					MouseWheelSensitivity * Mathf.Clamp(transform.position.magnitude / 15, 0.1f, 3) :
					MouseWheelSensitivity * Mathf.Clamp(transform.position.magnitude / 15, 1, 3);
					
					Vector3 npos = transform.position + transform.TransformDirection(Vector3.forward) * (wheel * tMouseWheelSensitivity);
					if ((npos.magnitude > 1 || wheel < 0) && (npos.magnitude < 500 || wheel > 0))
					{
						transform.position = npos;
						Common.camdis += wheel * tMouseWheelSensitivity;
						wheelspeed = wheel;
					}
					
				}
				else
				{
					float tMouseWheelSensitivity = wheelspeed > 0 ?
					MouseWheelSensitivity * Mathf.Clamp(transform.position.magnitude / 10, 0.1f, 5) :
					MouseWheelSensitivity * Mathf.Clamp(transform.position.magnitude / 10, 1, 5);
					
					Vector3 npos = transform.position + transform.TransformDirection(Vector3.forward) * (wheelspeed * tMouseWheelSensitivity);
					if ((npos.magnitude > 1 || wheel < 0) && (npos.magnitude < 500 || wheel > 0))
					{
						Common.camdis += wheelspeed * tMouseWheelSensitivity;
						transform.position = npos;
						wheelspeed = Mathf.Lerp(wheelspeed, 0, 0.1f);
					}
				}
            }
        }
        else
        {
            // rotation
            if (Input.GetMouseButton(1))
            {
                float x = Input.GetAxis("Mouse X");
                float y = -Input.GetAxis("Mouse Y");

                Vector3 up = GeoStaticData.getup(transform.position);
                transform.LookAt(transform.position + transform.forward * 1, up);
                transform.Rotate(up * (x * turnspeed), Space.World);
                transform.Rotate(Vector3.right * (y * turnspeed));
            }
        }
    }
}
