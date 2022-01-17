using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
public class EventHandler : MonoBehaviour,IPointerClickHandler
{
	public void OnPointerClick(PointerEventData eventData)
    {
       print("点击了：："+this.name);
	   transform.parent.gameObject.SendMessage("closebox", false);
    }
	
}
