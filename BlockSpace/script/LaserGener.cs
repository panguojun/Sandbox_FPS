using UnityEngine;
using System.Collections;

public class LaserGener : MonoBehaviour {
    public GameObject LASER;
	// Use this for initialization
	void Start () {
	    
	}
	
	// Update is called once per frame
	void Update () {
	
	}
    void shoot(Vector3 target, float actionalpha = 0)
    {
        StartCoroutine(DelayToInvoke.Invoke(() =>
        {            
            {
                LASER.SetActive(true);
                float scl = actionalpha < 0.5 ? Mathf.Lerp(0, 1, actionalpha / 0.5f) : Mathf.Lerp(1, 0, (actionalpha - 0.5f) / 0.5f);
                Vector3 forward = Vector3.zero;
                Vector3 forward2 = LASER.transform.InverseTransformPoint(target);
                PMDLL.line3D(LASER, forward, forward2, Camera.main.transform.position, 0.00001f + scl * 0.000618f);
            }
            actionalpha += Time.deltaTime * 1.0f;
            if (actionalpha < 1)
                shoot(target, actionalpha);
            else
            {
               LASER.SetActive(false);
            }
        }, 1 / 40.0f));
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
                if (Common.mouseselect(ray, out hit) && (hit.collider.gameObject != gameObject) && hit.collider.gameObject.name.Contains("Coelenterate"))
                {                   
                    shoot(hit.point);
                    Common.settreated(true, gameObject);
                }
            }
        }
    }
}
