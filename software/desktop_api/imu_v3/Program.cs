using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.ComponentModel;
using System.IO.Ports;

namespace imu_v3
{
    static class Program
    {
        static SerialPort mySerialPort;
        static string allReceivedData;
        static bool receivedFlag = false;
        static AHRS.MadgwickAHRS AHRS = new AHRS.MadgwickAHRS(1f / 100f, 0.1f);

        static float deg2rad(float degrees)
        {
            return (float)(Math.PI / 180) * degrees;
        }

        static void Main(string[] args)
        {

            string[] ports = SerialPort.GetPortNames();

            mySerialPort = new SerialPort(ports[0]);

            mySerialPort.BaudRate = 115200;
            mySerialPort.Parity = Parity.None;
            mySerialPort.StopBits = StopBits.One;
            mySerialPort.DataBits = 8;
            mySerialPort.Handshake = Handshake.None;
          

            mySerialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

            mySerialPort.Open();
            Console.WriteLine("Searching for x-IMU...");
            Form_3Dcuboid form_3DcuboidA = new Form_3Dcuboid(new string[] { "Form_3Dcuboid/RightInv.png", "Form_3Dcuboid/LeftInv.png", "Form_3Dcuboid/BackInv.png", "Form_3Dcuboid/FrontInv.png", "Form_3Dcuboid/TopInv.png", "Form_3Dcuboid/BottomInv.png" });
            form_3DcuboidA.Text += " A";

           



            BackgroundWorker backgroundWorkerA = new BackgroundWorker();
            backgroundWorkerA.DoWork += new DoWorkEventHandler(delegate { form_3DcuboidA.ShowDialog(); });
            backgroundWorkerA.RunWorkerAsync();



  
            

            while(true)
            {
                if (receivedFlag)
                {
                    receivedFlag = false;
                    x_IMU_API.QuaternionData zumo = new x_IMU_API.QuaternionData(AHRS.Quaternion);
                    form_3DcuboidA.RotationMatrix = zumo.ConvertToRotationMatrix();
                }
            }
            
            mySerialPort.Close();
           
        }

        private static void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;

            allReceivedData += sp.ReadExisting();
            while (allReceivedData.IndexOf('\n') != -1)
            {
                string oneLine = allReceivedData.Substring(0, allReceivedData.IndexOf('\n'));
                allReceivedData = allReceivedData.Remove(0, allReceivedData.IndexOf('\n') + 1);

                if (oneLine.IndexOf("DData") != -1)
                {
                    float[] data = new float[6];
                    string[] toParse = oneLine.Split(' ');

                    for(int i = 1; i < toParse.Length; i++)
                    {
                        string dd = toParse[i];
                        data[i-1] = (float)double.Parse(dd.Replace('.', ','));
                    }
                    AHRS.Update(deg2rad(data[0]), deg2rad(data[1]), deg2rad(data[2]), data[3], data[4], data[5]);
                    receivedFlag = true;
                    Console.WriteLine(oneLine);
                }
                
            }
            
        }


       
    }
}
