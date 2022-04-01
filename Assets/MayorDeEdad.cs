using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MayorDeEdad : MonoBehaviour
{
    public int Edad;
    public Text myText;
    // Start is called before the first frame update
    void Start()
    {
        if (Edad >= 18){
            myText.text = "Es mayor de edad";
        }else{
            myText.text = "No es mayor de edad";
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
