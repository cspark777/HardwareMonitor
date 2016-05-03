using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace xProtectorServer
{
    public partial class Form1 : Form
    {
        //Cria alguns eventos para saber que um cliente foi conectado
        //Cria um evento para saber que uma mensagem foi recebida
        //Cria um método para mandar a resposta para a mensagem daquele cliente
        TCPSocketServer mainServer = new TCPSocketServer(45000, 100);
        public delegate void AddClientCallback(string cliente);
        public delegate void ServerLogCallBack(string Log);


        public Form1()
        {
            InitializeComponent();
            mainServer.ClientConnected += new TCPSocketServer.SocketConnectedHandler(ClienteConectado);
            mainServer.MessageReceived += new TCPSocketServer.SocketMessageReceivedHandler(mainServer_MessageReceived);
        }


        //Message received
        void mainServer_MessageReceived(TCPSocketServer socketServer, SocketMessageReceivedArgs e)
        {
            if (this.txtServerLog.InvokeRequired)
            {
                this.txtServerLog.Invoke(new ServerLogCallBack(this.serverLog), new object[] {"Mensagem retornada:\n" +  e.MessageContent + "\n" });
            }
            else
            {
                txtServerLog.AppendText(e.MessageContent);
            }
        }

        //insere informação no log
        private void serverLog(string log)
        {
            txtServerLog.AppendText(log);
        }


        private void button1_Click(object sender, EventArgs e)
        {
            //envia mensagem para o cliente selecionado..
            if (lstConectados.Items.Count > 0 || lstConectados.SelectedIndex !=0 ){
            string[] selecionado = lstConectados.SelectedItem.ToString().Split(':');
            int id = int.Parse( selecionado[0]);

            mainServer.SendMessage(txtCommand.Text,id);
            }else{
                MessageBox.Show ("Selecione um cliente conectado antes de enviar o commando....");
            }
        }


        //Primary Server Controller Actions
        private void ClienteConectado(TCPSocketServer server, SocketConnectArgs e){
            if (this.lstConectados.InvokeRequired)
            {
                // It's on a different thread, so use Invoke
                //envia uma mensagem de boas vindas ao cliente
                server.SendMessage("Welcome to xprotector Sockets server\nWaiting credentials.", e.clientID);

                this.Invoke(new AddClientCallback(this.AddClient), new object[]{e.clientID + ":" + e.ConnectedIP.ToString()});
                
            }
            else{
                lstConectados.Items.Add("Cliente conectado: " + e.ConnectedIP.ToString());
            }
        }


        private void AddClient(string cliente)
        {
            this.lstConectados.Items.Add(cliente);
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void getHardware_Click(object sender, EventArgs e)
        {
            mainServer.Send_Reqiurement_GetHardwareInformation();
        }

        private void startsniff_Click(object sender, EventArgs e)
        {
            mainServer.Send_Requriement_StartSoftwareSniff();
        }

        private void StopSoftwareSniff_Click(object sender, EventArgs e)
        {
            mainServer.Send_Requriement_StopSoftwareSniff();
        }
    }
}
