%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  Gazepoint sample program for API communication
% Written in 2013 by Gazepoint www.gazept.com
%
% To the extent possible under law, the author(s) have dedicated all copyright 
% and related and neighboring rights to this software to the public domain worldwide. 
% This software is distributed without any warranty.
%
% You should have received a copy of the CC0 Public Domain Dedication along with this 
% software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% setup address and port
client_socket = tcpip('127.0.0.1', 4242); 

% setup line terminator
set(client_socket, 'InputBufferSize', 4096); 
fopen(client_socket); 
client_socket.Terminator = 'CR/LF';

% configure data server 
fprintf(client_socket, '<SET ID="ENABLE_SEND_COUNTER" STATE="1" />');
fprintf(client_socket, '<SET ID="ENABLE_SEND_POG_FIX" STATE="1" />');

% start data server sending data
fprintf(client_socket, '<SET ID="ENABLE_SEND_DATA" STATE="1" />');

pause(0.5);

tstart=tic;
% Loop for 10 seconds - print result to screen
while toc(tstart) < 10
while (get(client_socket, 'BytesAvailable') > 0) 
DataReceived = fscanf(client_socket)
% Parse 'DataReceived' string to extract data of interest
end 
pause(0.01) % delay so text can be printed on screen
end

% clean up
fclose(client_socket); 
delete(client_socket); 
clear client_socket 