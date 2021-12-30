using UnityEngine;
using System.Collections;
public class MeshCreatorEx : MonoBehaviour
{
	public int meshtype = 0;
	public int data = 0;
	public int state = 0;
	public int entindex = 0;
	
	void Start()
	{
		if (meshtype == -1)
			return;

		if (libPMDLL.createMesh (this.gameObject, meshtype, entindex, data, 0)){
			MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider> ();
            if (mc && meshtype > 100)
            {
                Mesh m = gameObject.GetComponent<MeshFilter>().mesh;
                mc.sharedMesh = m;
             
            }
			state = 1;
		}
		else
			state = -1;
	}
	void Update () 
	{
		if (meshtype == -1)
			return;
		
		if (meshtype > 100)
			return;
		
        updatemesh();
	}
	void updatemesh()
	{
		 if (libPMDLL.createMesh(this.gameObject, meshtype, entindex, data, 1))
            {
                MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
                if (mc)
                {
                    Mesh m = gameObject.GetComponent<MeshFilter>().mesh;
                    mc.sharedMesh = m;
                }
                state = 1;
            }
            else
                state = -1;
	}
    void OnGUI()
    {
        Event e = Event.current;
        // if (e.isMouse)
        // {
            // if (e.isMouse && Input.GetMouseButtonDown(2))
            // {
                // if (libPMDLL.createMesh(this.gameObject, meshtype, entindex, data, 1))
                // {
                    // MeshCollider mc = (MeshCollider)this.GetComponent<MeshCollider>();
                    // if (mc)
                    // {
                        // Mesh m = gameObject.GetComponent<MeshFilter>().mesh;
                        // mc.sharedMesh = m;
                    // }
                    // state = 1;
                // }
                // else
                    // state = -1;
            // }
        // }
        if (e.isKey && e.keyCode == KeyCode.Space)
        {
            updatemesh();
        }
    }
}