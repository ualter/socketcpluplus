package socket.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 */
public class SocketClient  {
	
	private String host;
	private int port;
	private Socket clientSocket;
	private PrintWriter out;
    private BufferedReader in;
	
    public static void main( String[] args )
    {
        SocketClient socketClient = new SocketClient();
        socketClient.init("192.168.0.22", 8888);
        for (int i = 0; i < 3; i++) {
        	socketClient.sendMessage("UALTER");
            socketClient.sendMessage("JUNIOR");
		}
        socketClient.closeConnection();
    }
    
    public SocketClient() {
    }
    
    public void init(String url, int port) {
    	this.host = url;
    	this.port = port;
    	try {
    		this.clientSocket = new Socket(this.host, this.port);
			this.out = new PrintWriter(clientSocket.getOutputStream(), true);
			this.in  = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			
			in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
	        System.out.println(in.readLine());
			
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
    }
    
    public String sendMessage(String msg) {
        out.println(msg);
        String resp;
		try {
			resp = this.in.readLine();
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
        return resp;
    }
 
    public void closeConnection() {
        try {
			in.close();
			out.close();
	        clientSocket.close();
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
        
    }
}
