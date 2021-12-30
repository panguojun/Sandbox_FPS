using UnityEngine;
using System.Collections;

public class luopan : MonoBehaviour {

    // --------------------------------
    // Use this for initialization
    private void Start()
    {
        //this.gameObject.SetActive(false);
        operstate = 0;
    }
    // --------------------------------
    // Update is called once per frame
    private void Update()
    {
        updatetransform();
    }
    // --------------------------------
    private void updatetransform()
    {
     //   transform.LookAt(Camera.main.transform.position);

        Vector2 sp = Camera.main.WorldToViewportPoint(this.gameObject.transform.position);
        float sz = this.gameObject.transform.lossyScale.magnitude;
        float tan = Mathf.Tan(Camera.main.fieldOfView / 2);
        float dd = Vector3.Distance(this.gameObject.transform.position, Camera.main.transform.position);
        sz = Mathf.Abs(sz / dd * (1 / tan) * Screen.width) / 2;

        MeshRenderer mr = gameObject.GetComponent<MeshRenderer>();
        mr.material.SetFloat("_CircleRadius", sz);
        mr.material.SetFloat("_CirclePosX", sp.x);
        mr.material.SetFloat("_CirclePosY", sp.y);
        //Debug.Log("sp = " + sp.ToString());   
    }
    // --------------------------------
    // GUI
    // --------------------------------
    public int operstate = 0; // 0 默认状态 1 界面已经出现
    void hittest(Ray ray)
    {       
        RaycastHit hit;
        if (Common.mouseselect(ray, out hit) && hit.collider.gameObject == gameObject)
        {
            Debug.Log("Raycast");
            MeshCollider collider = hit.collider as MeshCollider;
            if (collider == null || collider.sharedMesh == null)
                return;
            Debug.Log("Hit luopan!");
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
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                hittest(ray);
            }
        }
    }
}
