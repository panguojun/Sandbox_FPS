using UnityEngine;
using System.Collections;
public class Rover : MonoBehaviour {
    public GameObject CANVAS;
    public GameObject CAMCTRL;

    public GameObject OPERWINBTN;
    public GameObject EXITBTN;

    public GameObject ROVECTRL;

    GameObject portalbox;

    private int drivestate = 0; // 0 默认 1 坐下     

    // ---------------------------------------------------
    // 切回主控窗口
    public void onReturn()
    {
        CANVAS.SetActive(false);
        CAMCTRL.SetActive(false);

        PMDLL.invNorm(gameObject.GetComponent<MeshFilter>().mesh);
        ((Collider)this.GetComponent<Collider>()).enabled = true;
        ROVECTRL.SetActive(true);

        OPERWINBTN.SetActive(true);
        EXITBTN.SetActive(true);

        Camera.main.transform.rotation = this.transform.rotation;
    }
    void toOperWin()
    {
        CANVAS.SetActive(true);
        CAMCTRL.SetActive(true);

        PMDLL.invNorm(gameObject.GetComponent<MeshFilter>().mesh);
        ((Collider)this.GetComponent<Collider>()).enabled = false;
        ROVECTRL.SetActive(false);

        OPERWINBTN.SetActive(false);
        EXITBTN.SetActive(false);
    }
    void Start () {
        CANVAS.SetActive(false);
        CAMCTRL.SetActive(false);

        PMDLL.createMesh(this.gameObject, 2);       
        MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
        mc.sharedMesh = gameObject.GetComponent<MeshFilter>().mesh;

        //enter();
       // drivestate = 1;
    }
	void enter()
    {
        PMDLL.invNorm(gameObject.GetComponent<MeshFilter>().mesh);

        Vector3 up = transform.up;
        Camera.main.transform.position = this.transform.position + up * 0.5f;
        Camera.main.transform.rotation = this.transform.rotation;
        Camera.main.transform.SetParent(transform);

        {
            portalbox = GameObject.Find("portalbox");
            if (portalbox)
            {
                portalbox.SendMessage("closebox");
                portalbox.SetActive(false);     
            }
            {// 关闭手电
                GameObject go = GameObject.Find("Spotlight");
                if (go)
                    go.SetActive(false);
            }
        }
    }
    void outside()
    {
        if (drivestate != 1)
            return;
        PMDLL.invNorm(gameObject.GetComponent<MeshFilter>().mesh);

        Vector3 up = transform.up;
        Vector3 right = transform.right;
        Camera.main.transform.position = transform.position + up * 1 + right * 1;
        Camera.main.transform.LookAt(transform.position);
        Camera.main.transform.SetParent(null);

        gameObject.SendMessage("stop");
        {           
            if (portalbox)            
                portalbox.SetActive(true);
        }
    }

    // --------------------------------
    // GUI
    // --------------------------------
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
                    if (hit.collider.gameObject == OPERWINBTN)
                    {
                        toOperWin();
                    }
                    if (hit.collider.gameObject == EXITBTN && drivestate == 1)
                    {
                        outside();
                        drivestate = 0;
                        freemod.movementlocked = false;
                    }                    
                    else if (drivestate == 0 && hit.collider.gameObject == gameObject)
                    {
                        enter();
                        drivestate = 1;
                        freemod.movementlocked = true;
                    }                    
                }               
            }
        }
    }
}
