import paramiko
import os





def main():
    folder = "Spherical-Harmonic-Visualizer/Outputs"
    remoteFolder = "classes/phys5810/"+folder
    localFolder = "C:\\Users\\boucheri\\Source\\Classes\\phys5810\\"+folder

    hostname = 'sftp.osc.edu'
    port = 22
    username = 'YOUR_USERNAME'
    password = 'YOUR_PASSWORD'

    # Initialize SSH client
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())

    os.makedirs(localFolder, exist_ok=True)

    client.connect(hostname, port, username, password, timeout=5)

    with client.open_sftp() as sftp_client:
        for file in sftp_client.listdir(remoteFolder):
            cont = False
            for ext in ['.docx', '.zip', '.o', '.x', '.tmp']:
                if ext in file:
                    cont = True
            if cont:
                continue

            local_filepath = os.path.join(localFolder, file)
            remote_filepath = remoteFolder + '/' + file

            print("Remote Location: " + remote_filepath)
            print("Local Location: " + local_filepath)

            # Download the file from the remote server to the local system
            sftp_client.get(remote_filepath, local_filepath)

            print(f"File uploaded to {remote_filepath} successfully!")
            print('------------------------------------------------------')

    client.close()




if __name__ == "__main__":
    main()