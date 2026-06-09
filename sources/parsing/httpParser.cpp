/*
-le serv ne va pas recevoir toute la request d'un coup, donc tu dois 
	call parse jusqu'a que tu soies sur que la request est complete
-figure out comment savoir que la request est complete

function parse(data):
    buffer += data
    
    if state == START_LINE:
        if buffer has \r\n:
            extract first line
            split into method, path, version
            state = HEADERS
    
    if state == HEADERS:
        while buffer has complete lines:
            if line is empty (\r\n):
                state = BODY or COMPLETE
                break
            else:
                add header to map
    
    if state == BODY:
        if buffer size >= content_length:
            body = first content_length bytes
            state = COMPLETE
    
    return state == COMPLETE
*/