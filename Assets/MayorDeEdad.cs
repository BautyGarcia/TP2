using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MayorDeEdad : MonoBehaviour
{
    ////
    string edadString;
    int edadInt;
    public Text IngresoTexto;
    public Text textoCartelito;
    public GameObject Cartel_mayorDeEdad;
    // Start is called before the first frame update
    void Start()
    {
        Cartel_mayorDeEdad.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void ValidarEdad()
    {
        edadString = IngresoTexto.text;
        int.TryParse(edadString, out edadInt);

        if (edadInt >= 18){

            Cartel_mayorDeEdad.SetActive(true);
            textoCartelito.text = "Sos mayor de edad";
            
        }else{

            Cartel_mayorDeEdad.SetActive(true);
            textoCartelito.text = "Sos menor de edad";
        }
    }
}
