import socket
import json
import sys
import time # Import time module

def send_position_request(server_ip, server_port, node_id):
    """
    Connects to the OMNeT++ Orchestrator, sends a 'position' JSON request
    for a specified node, and prints the received response.

    Args:
        server_ip (str): The IP address of the OMNeT++ Orchestrator.
        server_port (int): The port the Orchestrator is listening on.
        node_id (int): The ID of the node whose position is requested.
    """
    sock = None # Initialize sock to None
    try:
        # 1. Create a TCP/IP socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Set a timeout for socket operations (connect, send, recv)
        # This prevents the client from hanging indefinitely if the server is unresponsive.
        sock.settimeout(10) # 10-second timeout for connect and receive operations

        print(f"\n--- Processing Request for Node ID: {node_id} ---")
        print(f"Attempting to connect to OMNeT++ Orchestrator at {server_ip}:{server_port}...")
        # 2. Connect to the server
        sock.connect((server_ip, server_port))
        print(f"Successfully connected to {server_ip}:{server_port}")

        # 3. Prepare the JSON request data
        request_data = {
            "request": "position",
            "node": node_id
        }

        json_request_string = json.dumps(request_data) + "\n"

        # 4. Encode and send the JSON request
        print(f"Sending request: {json_request_string.strip()}") # .strip() removes the newline for printing
        sock.sendall(json_request_string.encode('utf-8'))
        print("Request sent. Waiting for response...")

        # Add a short delay here to give OMNeT++ a chance to process the request
        # before the Python client's recv might timeout or the script completes.
        time.sleep(0.1) # Wait 100 milliseconds for server processing

        # 5. Receive the response from the server.
        response_buffer = b""
        while True:
            # Read data in chunks (e.g., 1024 bytes at a time)
            data = sock.recv(1024)
            if not data:
                # If recv returns an empty bytes object, the server has closed the connection.
                print("Server closed the connection unexpectedly (recv returned 0 bytes).")
                break # Connection closed
            response_buffer += data
            # Check if a newline character is present in the buffer.
            # This indicates the end of a complete message from the server.
            if b'\n' in response_buffer:
                print("Newline found in response buffer.")
                break # Message received, exit loop

        response = response_buffer.decode('utf-8').strip()
        print(f"Received response for Node {node_id}: {response}")

        # Add a longer delay here to keep the connection open for observation
        # This delay is per-request, meaning the connection for *this* node_id
        # will stay open for this duration after receiving its response.
        print(f"Client finished receiving for Node {node_id}. Keeping connection open for 1 second for observation...")
        time.sleep(1) # Keep connection open for 1 second before closing for this request

    except socket.timeout:
        print(f"Error: Connection or data reception timed out for Node {node_id}.", file=sys.stderr)
        print("Please ensure the OMNeT++ Orchestrator is running and responsive.", file=sys.stderr)
    except ConnectionRefusedError:
        print(f"Error: Connection refused for Node {node_id}. Is the OMNeT++ Orchestrator running and listening on {server_ip}:{server_port}?", file=sys.stderr)
        print("Check server status and firewall settings.", file=sys.stderr)
    except json.JSONDecodeError:
        print(f"Error: Could not decode JSON response for Node {node_id}. Received: '{response_buffer.decode('utf-8').strip()}'", file=sys.stderr)
    except Exception as e:
        print(f"An unexpected error occurred for Node {node_id}: {e}", file=sys.stderr)
    finally:
        # Ensure the socket is closed even if an error occurs.
        if sock:
            sock.close()
            print(f"Client: Connection for Node {node_id} closed.")

if __name__ == "__main__":
    # Configuration for the OMNeT++ Orchestrator
    OMNET_ORCHESTRATOR_IP = "127.0.0.1"  # Localhost IP
    OMNET_ORCHESTRATOR_PORT = 9998       # Port your Orchestrator listens on

    # List of node IDs to query
    nodes_to_query = [0, 1, 2]

    # Delay between consecutive requests (in seconds)
    delay_between_requests = 2.0 # Adjust as needed

    print("Starting OMNeT++ Orchestrator client for multiple nodes...")

    for node_id in nodes_to_query:
        send_position_request(OMNET_ORCHESTRATOR_IP, OMNET_ORCHESTRATOR_PORT, node_id)
        if node_id < nodes_to_query[-1]: # Don't delay after the last request
            print(f"\n--- Waiting {delay_between_requests} seconds before next request ---")
            time.sleep(delay_between_requests)

    print("\nAll position requests sent and processed.")
