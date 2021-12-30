using UnityEngine;
using System.Collections;

public class RobotArm : MonoBehaviour {

	// Use this for initialization
	void Start () {
        updatemesh();
    }

    bool isshooting = false;
	void Update () {
        if(!isshooting)
            updatemesh();
    }
    void updatemesh()
    {
        Vector3 forward = this.transform.InverseTransformDirection(this.transform.forward);
        PMDLL.robotArm(this.gameObject, 
            forward,
            forward + this.transform.up * (0.5F + 0.1F * Mathf.Sin(Time.time * 1.1F)),
            0.5f, 0.5F + 0.1F * Mathf.Sin(Time.time * 0.5F),
            0
            );
    }
    // --------------------------------------------------  
    void catchit(GameObject go)
    {

    }
    // --------------------------------------------------    
    void shoot(Vector3 target, float actionalpha = 0)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {
            isshooting = true;
            {
                float scl = actionalpha < 0.5 ? Mathf.Lerp(0, 1, actionalpha / 0.5f) : Mathf.Lerp(1, 0, (actionalpha - 0.5f) / 0.5f);
                Vector3 forward = this.transform.InverseTransformDirection(this.transform.forward);
                Vector3 forward2 = this.transform.InverseTransformDirection((target - this.gameObject.transform.position).normalized);
                PMDLL.robotArm(this.gameObject, forward, forward2, 0.5f, (target - this.gameObject.transform.position).magnitude * scl - 0.5f, actionalpha);
            }
            actionalpha += Time.deltaTime * 1.0f;
            if (actionalpha < 1)
                shoot(target, actionalpha);
            else
            {
                isshooting = false;
            }
        }, 1 / 60.0f));
    }
    // --------------------------------------------------    
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
                if (Common.mouseselect(ray, out hit) && (hit.collider.gameObject != gameObject && hit.collider.gameObject.name.Contains("Mine")))
                {
                    shoot(hit.point);
                    Common.settreated(true, gameObject, 788);
                }
            }           
        }
    }
}
