import json

def convert_date(t):
    return t.strftime('%m/%d/%Y %H:%M:%S')

def decorate_message(extra={}):  
    extra["API version"] ="1.0.0"
    extra["Docs"] = "https://github.com/israel-at-aritsugi-lab/rice-iot"  
    return {"messages":["Success"],"extra":extra}

def create_error_message(message):
    return json.dumps({"Message":message}),400, {'ContentType':'application/json'}

def bad_request():
    return "BAD REQUEST",400
