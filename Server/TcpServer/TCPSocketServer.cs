using System; 
using System.Collections.Generic; 
using System.Linq; 
using System.Text; 
using System.Net.Sockets; 
using System.Net;
using System.Net.Configuration;
using System.Data.SqlClient;
using xProtectorServer;


namespace xProtectorServer
{
    public class TCPSocketServer : IDisposable
    {
        const int GUID_LEN = 36;
        const int CLIENT_PORTNUM = 8888;

        /// <summary>
        ///  define constant for packet type (commands)
        /// </summary>


        const int MAX_PACKET_BUFFER_LEN = 0x100000;

        const int SERVER_REQUIRE = 0x100;
        const int CLIENT_RESPONSE = 0x200;
        const int CLIENT_NOTIFICATION = 0x300;
        const int SERVER_RESPONSE = 0x400;

        const int SERVER_REQUIRE_GET_HARDWARE_INFO = SERVER_REQUIRE + 1;
        const int SERVER_REQUIRE_CLIENT_START_SOFTWARE_SNIFF = SERVER_REQUIRE + 2;
        const int SERVER_REQUIRE_CLIENT_STOP_SOFTWARE_SNIFF = SERVER_REQUIRE + 3;
        const int SERVER_REQUIRE_SET_HARDWARE_PERMISSIONS = SERVER_REQUIRE + 4;
        const int SERVER_REQUIRE_SET_SOFTWARE_PERMISSIONS = SERVER_REQUIRE + 5;

        const int CLIENT_RESPONSE_AUTH_KEY_FAIL = CLIENT_RESPONSE + 0x80;
        const int CLIENT_RESPONSE_MACHINE_DATA = CLIENT_RESPONSE + 1;
        const int CLIENT_RESPONSE_SET_HARDWARE_PERMISSIONS_RESULT = CLIENT_RESPONSE + 2;
        const int CLIENT_RESPONSE_SET_SOFTWARE_PERMISSIONS_RESULT = CLIENT_RESPONSE + 3;
 
        const int CLIENT_NOTIFICATION_REGISTER = CLIENT_NOTIFICATION + 1;
        const int CLIENT_NOTIFICATION_LOGIN = CLIENT_NOTIFICATION + 2;
        const int CLIENT_NOTIFICATION_KEYBOARD_EVENT = CLIENT_NOTIFICATION + 3;
        const int CLIENT_NOTIFICATION_KEYLOGGER_DETECTED = CLIENT_NOTIFICATION + 4;   
        const int CLIENT_NOTIFICATION_MEMORY_SERIAL_CHANGED = CLIENT_NOTIFICATION + 5;
        const int CLIENT_NOTIFICATION_SOFTWARE_RUNNING = CLIENT_NOTIFICATION + 6;
 
        const int SERVER_RESPONSE_WELCOME = SERVER_RESPONSE + 0x80;
        const int SERVER_RESPONSE_REGISTERED = SERVER_RESPONSE + 1;
 
        /// <summary>
        /// 
        /// </summary>
        /// 

        private string m_guid;
        private string m_authKey;
        
        ///

        private const int MaxLengthOfPendingConnectionsQueue = 1000;
        private int portNumber;
        private int connectionsLimit;
        private Socket connectionSocket;
        private Dictionary<int, StateObject> connectedClients = new Dictionary<int, StateObject>();

        public event SocketConnectedHandler ClientConnected;
        public delegate void SocketConnectedHandler(TCPSocketServer socketServer, SocketConnectArgs e);
        public event SocketMessageReceivedHandler MessageReceived;
        public delegate void SocketMessageReceivedHandler(TCPSocketServer socketServer, SocketMessageReceivedArgs e);
        public event SocketClosedHandler ClientDisconnected;
        public delegate void SocketClosedHandler(TCPSocketServer socketServer, SocketEventArgs e);

        #region Constructor
        public TCPSocketServer(int PortNumber, int ConnectionsLimit = 0)
        {
            // database
            GetConnection();    


            // Validate Port Number 
            if (PortNumber > 0 && PortNumber < 65536)
            {
                this.portNumber = PortNumber;
            }
            else
            {
                throw new Exception("Ports number must be in the 1-65535 range. Note: 256 and bellow are normally reserved.");
            }

            this.connectionsLimit = ConnectionsLimit;
            startListening();
        }
        #endregion

        #region form event

        void Command_GetHardwareInfoRequest(Int64 commandId)
        {
            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlDataReader dataReader;

            command.CommandText = "select currentIP , AuthorizationKey from machines;";
            command.CommandType = System.Data.CommandType.Text;
            dataReader = command.ExecuteReader();

            byte[] authKey = new byte[256];

            while (dataReader.Read())
            {
                try
                {
                    if (dataReader.IsDBNull(0)) continue;

                    string s_ip = dataReader.GetString(0); ; //guid                     
                    int auth_len = (int)dataReader.GetBytes(1, 0, authKey, 0, 256); //authkey
                    string s_authKey = ByteArrayToString(authKey, auth_len);
                    byte[] packet = new byte[0x100];

                    int len = makePacket_ServerGetHardwareInfoRequest(packet, commandId, s_authKey);
                    SendPacketToClient(packet, len, s_ip);

                }
                catch (Exception ex)
                {
                    //
                }
            }

            dataReader.Close();
        }

        void Command_StartSoftwareSniffRequest()
        {
            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlDataReader dataReader;

            command.CommandText = "select currentIP , AuthorizationKey from machines;";
            command.CommandType = System.Data.CommandType.Text;
            dataReader = command.ExecuteReader();

            byte[] authKey = new byte[256];

            while (dataReader.Read())
            {
                try
                {
                    if (dataReader.IsDBNull(0)) continue;

                    string s_ip = dataReader.GetString(0); ; //guid                     
                    int auth_len = (int)dataReader.GetBytes(1, 0, authKey, 0, 256); //authkey
                    string s_authKey = ByteArrayToString(authKey, auth_len);
                    byte[] packet = new byte[0x100];

                    int len = makePacket_StartSoftwareSniffRequest(packet, s_authKey);
                    SendPacketToClient(packet, len, s_ip);

                }
                catch (Exception ex)
                {
                    //
                }
            }

            dataReader.Close();
        }

        void Command_StopSoftwareSniffRequest()
        {
            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlDataReader dataReader;

            command.CommandText = "select currentIP , AuthorizationKey from machines;";
            command.CommandType = System.Data.CommandType.Text;
            dataReader = command.ExecuteReader();

            byte[] authKey = new byte[256];

            while (dataReader.Read())
            {
                try
                {
                    if (dataReader.IsDBNull(0)) continue;

                    string s_ip = dataReader.GetString(0); ; //guid                     
                    int auth_len = (int)dataReader.GetBytes(1, 0, authKey, 0, 256); //authkey
                    string s_authKey = ByteArrayToString(authKey, auth_len);
                    byte[] packet = new byte[0x100];

                    int len = makePacket_StopSoftwareSniffRequest(packet, s_authKey);
                    SendPacketToClient(packet, len, s_ip);

                }
                catch (Exception ex)
                {
                    //
                }
            }

            dataReader.Close();
        }


        public void Send_Reqiurement_GetHardwareInformation()
        {
            Int64 commandId = 0x1122334455667788;

            Command_GetHardwareInfoRequest(commandId);
        }

        public void Send_Requriement_StartSoftwareSniff()
        {
            Command_StartSoftwareSniffRequest();
        }

        public void Send_Requriement_StopSoftwareSniff()
        {
            Command_StopSoftwareSniffRequest();
        }

        #endregion

        #region tool

        string ByteArrayToString(byte[] ba,int len)
        {
            StringBuilder hex = new StringBuilder(ba.Length * 2);
            for (int i = 0; i < len; i++)
                hex.Append(ba[i].ToString("X2"));
            return hex.ToString();
        }

        void convertIntToByteArray(int val, byte[] buff, int pos)
        {
            buff[pos+3] = (byte)(val >> 24);
            buff[pos+2] = (byte)(val >> 16);
            buff[pos+1] = (byte)(val >> 8);
            buff[pos] = (byte)val;
        }

        void convertInt64ToByteArray(Int64 val, byte[] buff, int pos)
        {
            buff[pos + 7] = (byte)(val >> 56);
            buff[pos + 6] = (byte)(val >> 48);
            buff[pos + 5] = (byte)(val >> 40);
            buff[pos + 4] = (byte)(val >> 32);
            buff[pos + 3] = (byte)(val >> 24);
            buff[pos + 2] = (byte)(val >> 16);
            buff[pos + 1] = (byte)(val >> 8);
            buff[pos + 0] = (byte)val;
        }

        Int64 convertByteArrayToInt64(byte[] buff, int pos)
        {
            Int64 val = 0;

            for (int i = 7; i >=0 ; i--)
            {
                val = (val << 8) + buff[pos + i];
            }
            return val;
        }

        #endregion

        private StateObject GetClient(int clientId)
        {
            StateObject client;
            if (!connectedClients.TryGetValue(clientId, out client))
            {
                return null;
            }
            return client;
        }

        #region database stored procedure

        private static System.Data.SqlClient.SqlConnection m_connection = new System.Data.SqlClient.SqlConnection();        


        static void GetConnection()
        {
            m_connection.ConnectionString = "Data Source=data.port0.org;Integrated Security=false;Initial Catalog=xhardware;User ID=steven;Password=steveny2013";
            m_connection.Open();            
        }

        static int CallStoredProcedure_register_machine(string machineCredential,string machineip , byte[] authKey,byte[] guid)
        {
            //authkey : OUT
            //guid : OUT
            //return len of authkey

            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlDataReader dataReader;
            System.Data.SqlClient.SqlTransaction transaction;


            transaction = m_connection.BeginTransaction(System.Data.IsolationLevel.ReadCommitted);

            command.CommandText = "register_machine";
            command.CommandType = System.Data.CommandType.StoredProcedure;

            parameter = command.Parameters.AddWithValue("@machine_credentials", machineCredential);
            parameter = command.Parameters.AddWithValue("@IPAddress", machineip);             
            
            command.Transaction = transaction;
            dataReader = command.ExecuteReader();

            dataReader.Read();

            string s_guid = dataReader.GetString(0); //guid

            byte[] b_guid = Encoding.UTF8.GetBytes(s_guid);
            Array.Copy(b_guid, guid, GUID_LEN);

            long byte1 = dataReader.GetBytes(1, 0, authKey, 0, 256); //authkey
            dataReader.Close();
            
            transaction.Commit();
            
            return (int)byte1;
        }

        static int CallStoredProcedure_check_machine(string machineGuid, byte[] authKey)
        {
            //authkey : OUT
            //return : len of authKey

            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlDataReader dataReader;
            System.Data.SqlClient.SqlTransaction transaction;


            transaction = m_connection.BeginTransaction(System.Data.IsolationLevel.ReadCommitted);

            command.CommandText = "check_machine";
            command.CommandType = System.Data.CommandType.StoredProcedure;

            parameter = command.Parameters.AddWithValue("@machine_guid", machineGuid);
            
            command.Transaction = transaction;
            dataReader = command.ExecuteReader();

            dataReader.Read();            

            long byte1 = dataReader.GetBytes(0, 0, authKey, 0, 256); //authkey
            dataReader.Close();

            transaction.Commit();

            return (int)byte1;
        }

        static int CallStoredProcedure_add_machine_hardware(string machine_guid, string machine_name, string machine_ip, 
                                        string hardserial,string cpuserial, string mac, string total_memory,string total_harddisk,
                                        string used_harddisk, string operatingsystem, string processor, byte[] device_list)
        {
            //authkey : OUT
            //guid : OUT
            //return len of authkey
            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;
            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlTransaction transaction;

            transaction = m_connection.BeginTransaction(System.Data.IsolationLevel.ReadCommitted);

            command.CommandText = "add_machine_hardware";
            command.CommandType = System.Data.CommandType.StoredProcedure;

            parameter = command.Parameters.AddWithValue("@machine_guid", machine_guid);
            parameter = command.Parameters.AddWithValue("@Name", machine_name);
            parameter = command.Parameters.AddWithValue("@IP", machine_ip);
            parameter = command.Parameters.AddWithValue("@hdserial", hardserial);
            parameter = command.Parameters.AddWithValue("@cpuserial", cpuserial);
            parameter = command.Parameters.AddWithValue("@mac", mac);
            parameter = command.Parameters.AddWithValue("@TotalMemory", total_memory);
            parameter = command.Parameters.AddWithValue("@TotalHardisk", total_harddisk);
            parameter = command.Parameters.AddWithValue("@UsedHardDisk", used_harddisk);
            parameter = command.Parameters.AddWithValue("@OperatingSystem", operatingsystem);
            parameter = command.Parameters.AddWithValue("@Processor", processor);
            parameter = command.Parameters.AddWithValue("@DeviceList", device_list);

            command.Transaction = transaction;
            command.ExecuteNonQuery();

            transaction.Commit();

            return 1;
        }

        static int CallStoredProcedure_auth_key_failed(Int64 commandId)
        {

            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlTransaction transaction;


            transaction = m_connection.BeginTransaction(System.Data.IsolationLevel.ReadCommitted);

            command.CommandText = "auth_key_failed";
            command.CommandType = System.Data.CommandType.StoredProcedure;

            parameter = command.Parameters.AddWithValue("@commandID", commandId);
            
            command.Transaction = transaction;
            command.ExecuteNonQuery();

            transaction.Commit();

            return 1;
        }

        static int CallStoredProcedure_notification_keylogger_detected(string machine_guid)
        {

            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlTransaction transaction;


            transaction = m_connection.BeginTransaction(System.Data.IsolationLevel.ReadCommitted);

            command.CommandText = "notification_keylogger_detected";
            command.CommandType = System.Data.CommandType.StoredProcedure;

            parameter = command.Parameters.AddWithValue("@machineGuid", machine_guid);

            command.Transaction = transaction;
            command.ExecuteNonQuery();

            transaction.Commit();

            return 1;
        }

        static int CallStoredProcedure_notification_keyboard_event(string machine_guid,string keyboard_event)
        {

            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlTransaction transaction;


            transaction = m_connection.BeginTransaction(System.Data.IsolationLevel.ReadCommitted);

            command.CommandText = "notification_keyboard_event";
            command.CommandType = System.Data.CommandType.StoredProcedure;

            parameter = command.Parameters.AddWithValue("@machineGuid", machine_guid);
            parameter = command.Parameters.AddWithValue("@Event", keyboard_event);

            command.Transaction = transaction;
            command.ExecuteNonQuery();

            transaction.Commit();

            return 1;
        }

        static int CallStoredProcedure_notification_memory_changed(string machine_guid, string new_serial) //not using new serial now
        {

            System.Data.SqlClient.SqlCommand command = new System.Data.SqlClient.SqlCommand();
            command.Connection = m_connection;

            System.Data.SqlClient.SqlParameter parameter;
            System.Data.SqlClient.SqlTransaction transaction;


            transaction = m_connection.BeginTransaction(System.Data.IsolationLevel.ReadCommitted);

            command.CommandText = "notification_memory_changed";
            command.CommandType = System.Data.CommandType.StoredProcedure;

            parameter = command.Parameters.AddWithValue("@machineGuid", machine_guid);
            

            command.Transaction = transaction;
            command.ExecuteNonQuery();

            transaction.Commit();

            return 1;
        }

        #endregion

        #region Send Messages
        public void SendMessage(string MessageToSend, int clientID)
        {
            byte[] data = System.Text.Encoding.UTF8.GetBytes(MessageToSend + "\0");
            SendMessage(data, clientID);
        }

        public void SendMessage(byte[] MessageToSend, int clientID) //
        {
            StateObject client = GetClient(clientID);
            if (client != null)
            {
                try
                {
                    if (client.socket.Connected)
                    {
                        client.socket.Send(MessageToSend);
                    }
                }
                catch (SocketException)
                {
                    // Close socket 
                    closeSocket(clientID);
                }
            }
        }

        public void SendPacketToClient(byte[] bufToSend, int len, string clientIp) //
        {
            IPEndPoint ipep = new IPEndPoint(System.Net.IPAddress.Parse(clientIp), CLIENT_PORTNUM);
            Socket server = new Socket(AddressFamily.InterNetwork,SocketType.Stream, ProtocolType.Tcp);

            try
            {
                server.Connect(ipep);
                NetworkStream networkStream = new NetworkStream(server);
                networkStream.Write(bufToSend, 0, len);
                networkStream.Flush();
                server.Shutdown(SocketShutdown.Both);
                server.Close();
            }
            catch
            {

            }

        }

        #endregion

        
        #region process recieve packet region

        private void myProcessPacket(byte[] buffer, int len)
        {
            int command = BitConverter.ToInt32(buffer, 0);
            int fieldscount = BitConverter.ToInt32(buffer, 4);
            int field_len = 0;
            int pos = 8;
            byte[] packet = new byte[0x100];

            switch (command)
            {
                case CLIENT_NOTIFICATION_REGISTER: // packet 25
                    {
                        field_len = BitConverter.ToInt32(buffer, pos);
                        pos = pos + 4;
                        string str_machine_credential = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;
                        field_len = BitConverter.ToInt32(buffer, pos);
                        pos = pos + 4;
                        string str_current_ip = Encoding.UTF8.GetString(buffer, pos, field_len);

                        byte[] authkey = new byte[256];
                        byte[] guid = new byte[256];
                        int auth_len = CallStoredProcedure_register_machine(str_machine_credential, str_current_ip, authkey, guid);
                        string s_authKey = ByteArrayToString(authkey, auth_len);
                        string s_guid = Encoding.Default.GetString(guid, 0, GUID_LEN);

                        len = makePacket_ServerRegisterResponse(packet, s_guid, s_authKey);
                        SendPacketToClient(packet, len, str_current_ip);

                        break;
                    }
                case CLIENT_NOTIFICATION_LOGIN: // packet 27
                    {
                        field_len = BitConverter.ToInt32(buffer, pos);
                        pos = pos + 4;
                        string str_machine_guid = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;
                        field_len = BitConverter.ToInt32(buffer, pos);
                        pos = pos + 4;
                        string str_current_ip = Encoding.UTF8.GetString(buffer, pos, field_len);

                        byte[] authkey = new byte[256];
                        int auth_len = CallStoredProcedure_check_machine(str_machine_guid,  authkey);
                        string s_authKey = ByteArrayToString(authkey, auth_len);

                        len = makePacket_ServerLoginResponse(packet, s_authKey);
                        SendPacketToClient(packet, len, str_current_ip);

                        break;
                    }
                case CLIENT_RESPONSE_MACHINE_DATA: // packet 10
                    {

                        Int64 commandID = convertByteArrayToInt64(buffer, pos);
                        pos = pos + 8;

                        field_len = BitConverter.ToInt32(buffer, pos);  //machine guid
                        pos = pos + 4;
                        string str_machine_guid = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //machinename
                        pos = pos + 4;
                        string str_machine_name = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //machineip
                        pos = pos + 4;
                        string str_machine_ip = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //processor
                        pos = pos + 4;
                        string str_processor = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //Total memory
                        pos = pos + 4;
                        string str_total_memory = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //Total Harddisk
                        pos = pos + 4;
                        string str_total_harddisk = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //Used harddisk
                        pos = pos + 4;
                        string str_used_harddisk = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //operating system
                        pos = pos + 4;
                        string str_operating_system = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //Hardserial
                        pos = pos + 4;
                        string str_hard_serial = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //CPU serial
                        pos = pos + 4;
                        string str_cpu_serial = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //mac address
                        pos = pos + 4;
                        string str_mac_address = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //hardware checksum
                        pos = pos + 4; 
                        int hardware_checksum = BitConverter.ToInt32(buffer, pos);                        
                        string str_hardware_checksum = hardware_checksum.ToString("X");
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //credential
                        pos = pos + 4;
                        string str_credential = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos) * 9; //device list
                        pos = pos + 4;
                        byte[] device_list = new byte[field_len];
                        Array.Copy(buffer, pos, device_list, 0, field_len);

                        CallStoredProcedure_add_machine_hardware(str_machine_guid, str_machine_name, str_machine_ip, str_hard_serial, str_cpu_serial,
                            str_mac_address, str_total_memory, str_total_harddisk, str_used_harddisk, str_operating_system, str_processor, device_list);                        

                        break;
                    }
                case CLIENT_RESPONSE_AUTH_KEY_FAIL: // packet 8
                    {
                        Int64 commandID = convertByteArrayToInt64(buffer, pos);
                        pos = pos + 8;

                        CallStoredProcedure_auth_key_failed(commandID);
                        break;
                    }

                case CLIENT_NOTIFICATION_KEYBOARD_EVENT:
                    {
                        field_len = BitConverter.ToInt32(buffer, pos); //machine_guid
                        pos = pos + 4;
                        string str_guid = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //event
                        pos = pos + 4;
                        string str_event = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        CallStoredProcedure_notification_keyboard_event(str_guid,str_event);

                        break;
                    }

                case CLIENT_NOTIFICATION_KEYLOGGER_DETECTED:
                    {
                        field_len = BitConverter.ToInt32(buffer, pos); //machine_guid
                        pos = pos + 4;
                        string str_guid = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        CallStoredProcedure_notification_keylogger_detected(str_guid);
                        break;
                    }
                case CLIENT_NOTIFICATION_MEMORY_SERIAL_CHANGED:
                    {
                        field_len = BitConverter.ToInt32(buffer, pos); //machine_guid
                        pos = pos + 4;
                        string str_guid = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //machine_guid
                        pos = pos + 4;
                        string str_RAMserial = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        break;
                    }
                case CLIENT_NOTIFICATION_SOFTWARE_RUNNING:
                    {
                        field_len = BitConverter.ToInt32(buffer, pos); //machine_guid
                        pos = pos + 4;
                        string str_guid = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;

                        field_len = BitConverter.ToInt32(buffer, pos); //machine_guid
                        pos = pos + 4;
                        string str_RAMserial = Encoding.UTF8.GetString(buffer, pos, field_len);
                        pos = pos + field_len;
                        break;
                    }


                default:
                    break;

            }

        }

        #endregion

        #region make packet to client

        private int makePacket_ServerRegisterResponse(byte[] packet, string machine_guid, string auth_key)
        {
            int pos = 0;
            int len = 0;
            convertIntToByteArray(SERVER_RESPONSE_REGISTERED, packet, pos);
            pos = pos + 4;
            len = 2; // fields count
            convertIntToByteArray(len, packet, pos);
            pos += 4;

            len = machine_guid.Length;
            convertIntToByteArray(len, packet, pos);
            pos += 4;

            byte[] byte_machine_guid = Encoding.ASCII.GetBytes(machine_guid);
            Array.Copy(byte_machine_guid, 0, packet, pos, len);
            pos += len;

            len = auth_key.Length;
            convertIntToByteArray(len, packet, pos);
            pos += 4;
            byte[] byte_auth_key = Encoding.ASCII.GetBytes(auth_key);
            Array.Copy(byte_auth_key, 0, packet, pos, len);
            pos += len;

            return pos;
        }
        private int makePacket_ServerLoginResponse(byte[] packet, string auth_key)
        {
            int pos = 0;
            int len = 0;
            convertIntToByteArray(SERVER_RESPONSE_WELCOME, packet, pos);
            pos = pos + 4;
            len = 2; // fields count
            convertIntToByteArray(len, packet, pos);
            pos += 4;

            len = auth_key.Length;
            convertIntToByteArray(len, packet, pos);
            pos += 4;
            byte[] byte_auth_key = Encoding.ASCII.GetBytes(auth_key);
            Array.Copy(byte_auth_key, 0, packet, pos, len);
            pos += len;

            return pos;
        }
        private int makePacket_ServerGetHardwareInfoRequest(byte[] packet,Int64 commandID, string auth_key)
        {
            int pos = 0;
            int len = 0;
            convertIntToByteArray(SERVER_REQUIRE_GET_HARDWARE_INFO, packet, pos);
            pos = pos + 4;            

            len = 1; // fields count
            convertIntToByteArray(len, packet, pos);
            pos += 4;

            convertInt64ToByteArray(commandID, packet, pos);//command ID
            Int64 val = convertByteArrayToInt64(packet, pos);
            pos = pos + 8;

            len = auth_key.Length;
            convertIntToByteArray(len, packet, pos);
            pos += 4;

            byte[] byte_machine_guid = Encoding.ASCII.GetBytes(auth_key);
            Array.Copy(byte_machine_guid, 0, packet, pos, len);
            pos += len;

            return pos;
        }

        private int makePacket_StartSoftwareSniffRequest(byte[] packet,  string auth_key)
        {
            int pos = 0;
            int len = 0;
            convertIntToByteArray(SERVER_REQUIRE_CLIENT_START_SOFTWARE_SNIFF, packet, pos);
            pos = pos + 4;

            len = 1; // fields count
            convertIntToByteArray(len, packet, pos);
            pos += 4;
             
            len = auth_key.Length;
            convertIntToByteArray(len, packet, pos);
            pos += 4;

            byte[] byte_machine_guid = Encoding.ASCII.GetBytes(auth_key);
            Array.Copy(byte_machine_guid, 0, packet, pos, len);
            pos += len;

            return pos;
        }

        private int makePacket_StopSoftwareSniffRequest(byte[] packet, string auth_key)
        {
            int pos = 0;
            int len = 0;
            convertIntToByteArray(SERVER_REQUIRE_CLIENT_STOP_SOFTWARE_SNIFF, packet, pos);
            pos = pos + 4;

            len = 1; // fields count
            convertIntToByteArray(len, packet, pos);
            pos += 4;

            len = auth_key.Length;
            convertIntToByteArray(len, packet, pos);
            pos += 4;

            byte[] byte_machine_guid = Encoding.ASCII.GetBytes(auth_key);
            Array.Copy(byte_machine_guid, 0, packet, pos, len);
            pos += len;

            return pos;
        }
        #endregion

        #region Connection and Listening
        private void startListening()
        {
            try
            {
                // Create listening socket 
                connectionSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                connectionSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
                IPEndPoint ipLocal = new IPEndPoint(IPAddress.Any, this.portNumber);
                // Bind to local IP Address 
                connectionSocket.Bind(ipLocal);
                // Start Listening 
                connectionSocket.Listen(MaxLengthOfPendingConnectionsQueue);
                // Create callback to handle client connections 
                connectionSocket.BeginAccept(new AsyncCallback(onClientConnect), null);
            }
            catch (SocketException)
            {
                throw new Exception("Cannot listen on this socket. Fatal Error");
            }
        }

        private void onClientConnect(IAsyncResult asyn)
        {
            // Create a new StateObject to hold the connected client 
            StateObject connectedClient = new StateObject();
            try
            {
                connectedClient.socket = connectionSocket.EndAccept(asyn);
                connectedClient.id = !connectedClients.Any() ? 1 : connectedClients.Keys.Max() + 1;

                connectedClients.Add(connectedClient.id, connectedClient);

                // Check against limit 
                if (connectedClients.Count > connectionsLimit && connectionsLimit != 0)
                {
                    // No connection event is sent so close connection quietly 
                    closeSocket(connectedClient.id, true);
                    return;
                }

                // Allow connected client to receive data and designate a callback method 
                connectedClient.socket.BeginReceive(connectedClient.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(onReceivedClientData), connectedClient);
            }
            catch (Exception)
            {
                closeSocket(connectedClient.id, true);
            }
            finally
            {
                // Perfomed here to not get any exceptions on the main socket caught up in client connection errors. 
                ReleaseConnectionSocket();
            }

            /*
            // Dispatch Event at the end as any errors in socket dispatch silent diconnections 
            if (ClientConnected != null)
            {
                SocketConnectArgs args = new SocketConnectArgs()
                {
                    ConnectedIP = IPAddress.Parse(((IPEndPoint)connectedClient.socket.RemoteEndPoint).Address.ToString()),
                    clientID = connectedClient.id
                };
                ClientConnected(this, args);
                
            }
            */
        }

        private void ReleaseConnectionSocket()
        {
            try
            {
                // Release connectionSocket to keep listening 
                connectionSocket.BeginAccept(new AsyncCallback(onClientConnect), null);
            }
            catch (SocketException)
            {
                throw new Exception ("Cannot listen on the main socket. Fatal Error");
            }
        }        

        private void onReceivedClientData(IAsyncResult asyn)
        {
            // Receive stateobject of the client that sent data 
            StateObject dataSender = (StateObject)asyn.AsyncState;

            try
            {
                // Complete aysnc receive method and read data length 
                int bytesRead = dataSender.socket.EndReceive(asyn);

                if (bytesRead > 0)
                {
                    myProcessPacket(dataSender.buffer, bytesRead);
                    /*
                    // More data could be sent so append data received so far 
                    dataSender.sb.Append(Encoding.UTF8.GetString(dataSender.buffer, 0, bytesRead));
                    String content = dataSender.sb.ToString();
                    if (!string.IsNullOrEmpty(content))
                    {
                        String formattedMessage = content.Replace("\0", "");

                        // Dispatch Event 
                        if (MessageReceived != null)
                        {
                            SocketMessageReceivedArgs args = new SocketMessageReceivedArgs()
                            {
                                MessageContent = formattedMessage,
                                clientID = dataSender.id
                            };
                            MessageReceived(this, args);
                        }

                        dataSender.sb.Length = 0;
                    }
                     */
                    /*
                    try
                    {
                        dataSender.socket.BeginReceive(dataSender.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(this.onReceivedClientData), dataSender);
                    }
                    catch (SocketException)
                    {
                        closeSocket(dataSender.id);
                    }*/
                }
                else
                {
                    closeSocket(dataSender.id);
                }
            }
            catch (SocketException ex)
            {
                // Socket closed at other end 
                if (ex.ErrorCode == 10054)
                {
                    closeSocket(dataSender.id);
                }
                else
                {
                    closeSocket(dataSender.id);
                }
            }
        }
        #endregion

        #region Socket Closing
        public void closeSocket(int SocketID)
        {
            closeSocket(SocketID, false);
        }

        // This does not dispatch an event, this task is to be used when rejecting connections past the limit. 
        // No connection event is sent so no disconnection event should be sent. 
        private void closeSocket(int SocketId, bool silent)
        {
            StateObject client = GetClient(SocketId);
            if (client == null)
            {
                return;
            }
            try
            {
                client.socket.Close();
                //client.socket. Dispose();

                if (!silent)
                {
                    // Dispatch event 
                    if (ClientDisconnected != null)
                    {
                        SocketEventArgs args = new SocketEventArgs()
                        {
                            clientID = client.id
                        };
                        ClientDisconnected(this, args);
                    }
                }
            }
            catch (SocketException)
            {
                // Socket is being removed anyway. 
            }
            finally
            {
                connectedClients.Remove(client.id);
            }
        }

        public void closeAllSockets()
        {
            var keys = connectedClients.Keys;
            foreach (int key in keys)
            {
                var client = connectedClients[key];
                closeSocket(client.id);
            }
        }
        #endregion

        public void Dispose()
        {
            this.ClientConnected = null;
            this.ClientDisconnected = null;
            this.MessageReceived = null;

            connectionSocket.Close();
        }
    }
}
