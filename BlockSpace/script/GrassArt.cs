using UnityEngine;
using System.Collections;

public class GrassArt : MonoBehaviour {

	// Use this for initialization
	void Start () {
        PMDLL.createMesh(this.gameObject, 11);
    }
    void createGrass(Ray ray)
    {return;
        RaycastHit hit;
        if (Common.mouseselect(ray, out hit))
        {
            bool edited = PMDLL.createMeshEx(this.gameObject, 1, hit.point, hit.normal, 11);
            if (edited)
            {
                gameObject.SetActive(false);
                Mesh mesh0 = gameObject.GetComponent<MeshFilter>().mesh;                
                CombineInstance[] combine = new CombineInstance[2];
                combine[0].mesh = mesh0;
                combine[0].transform = Matrix4x4.identity;
                combine[1].mesh = new Mesh();
                combine[1].transform = Matrix4x4.identity;
                PMDLL.updateMesh(combine[1].mesh);

                Mesh newmesh = new Mesh();
                newmesh.CombineMeshes(combine);
                gameObject.GetComponent<MeshFilter>().mesh = newmesh;
                gameObject.SetActive(true);
            }
        }
    }
    
}
