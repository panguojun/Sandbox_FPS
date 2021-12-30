using UnityEngine;
using System.Collections;

public class MC2DLogic : MonoBehaviour {
    public GameObject CUBETEMP;
    public const float c_size = 0.1f;
    GameObject[,] matrix3x3 = new GameObject[3, 3];
    GameObject mousedownobj;

    void setup()
    {        
        Material mat0 = ((Renderer)CUBETEMP.GetComponent<Renderer>()).material;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                {
                if (Random.RandomRange(0, 100) < 50)
                {
                    GameObject bd = Instantiate(CUBETEMP);
                    bd.SetActive(true);
                    bd.transform.position = transform.position + transform.rotation * (new Vector3((i - 1) * c_size, (j - 1) * c_size, 0));
                    bd.transform.parent = transform;
                    matrix3x3[i, j] = bd;

                    Material mat = new Material(mat0);
                    mat.SetColor("_EmissionColor", new Color(Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), Random.Range(0.0f, 1.0f), 1));
                    ((Renderer)bd.GetComponent<Renderer>()).material = mat;
                }
                }
    }
    void updatePos(int i, int j)
    {
        matrix3x3[i, j].transform.position = transform.position + transform.rotation * (new Vector3((i - 1) * c_size, (j - 1) * c_size, 0));
    }
    void exchange(int i1, int j1, int i2, int j2)
    {
        GameObject go = matrix3x3[i2, j2];
        matrix3x3[i2, j2] = matrix3x3[i1, j1];
        matrix3x3[i1, j1] = go;
        updatePos(i1, j1);
        updatePos(i2, j2);
        Debug.Log("exchange " + i1.ToString());
    }
    bool getIndice(GameObject go, out int ii, out int jj)
    {
        for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if(go == matrix3x3[i, j])
            {
                ii = i;
                jj = j;
                return true;
             }
        }
        ii = -1;
        jj = -1;
        return false;
    }
    void exchange(GameObject go1, GameObject go2)
    {
        int i1, j1, i2, j2;
        if (!getIndice(go1, out i1, out j1) || !getIndice(go2, out i2, out j2))
            return;
        exchange(i1, j1, i2, j2);
    }
    // Use this for initialization
    void Start () {
        setup();
    }
	
	// Update is called once per frame
	void Update () {
       
    }
    //public string doorname = "Building_26_-12220";
    public GameObject door;
    void OnGUI()
    {
       // if(door == null)
       // {
       //     door = GameObject.Find(doorname);
       // }
        if (freemod.bfreewalk)
            return;
        Event e = Event.current;
        if (e.isMouse)
        {            
           // pick	
           if (Input.GetMouseButtonDown(0))
           {
               Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit rhit;
                if (Common.mouseselect(ray, out rhit) && rhit.collider.gameObject)
                {
                    mousedownobj = rhit.collider.gameObject;
                    Common.settreated(true, gameObject, 78);
               }
           }
            if (Input.GetMouseButtonUp(0))
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit rhit;
                if (Common.mouseselect(ray, out rhit) && rhit.collider.gameObject)
                {
                    if (mousedownobj)
                    {
                        exchange(mousedownobj, rhit.collider.gameObject);
                        if(door) door.SetActive(false);
                        mousedownobj = null;
                        Common.settreated(true, gameObject, 75);
                    }                   
                }
            }
        }
    }

    void OnBeforeApplicationQuit()
    {
        if(door) door.SetActive(true);
        Debug.Log("OnBeforeApplicationQuit");
    }
}
