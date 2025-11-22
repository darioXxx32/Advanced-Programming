"""
FastAPI server that shows a locked love notes page.
Run with: uvicorn server_lindo_mensaje:app --reload --port 8080
"""

from datetime import date

from fastapi import FastAPI, HTTPException
from fastapi.responses import HTMLResponse, JSONResponse, RedirectResponse
from pydantic import BaseModel

app = FastAPI(title="Mensaje para mi novia")

# --- Configuration ---
# Change this to the real date you empezaron a salir (YYYY-MM-DD)
ANNIVERSARY_DATE = date.fromisoformat("2024-12-01")
UNLOCK_NOTE = (
    "¡Feliz primer mes, mi amor! Aquí tienes las notitas que escribí pensando en ti."
)
RECIPIENT_NAME = "Ashly"
RECIPIENT_NICK = "Mi Koalita"

# --- The 12 personalized notes (edit the texts below if you want) ---
NOTES = [
    "Para ti, que haces que cada día brille un poquito más.",
    "Eres la razón por la que sonrío sin motivo. Te quiero mucho.",
    "Si el mundo fuera un jardín, tus risas serían las flores más bonitas.",
    "Gracias por tus abrazos, que arreglan todo en un segundo.",
    "Eres mi poema favorito en versos de a diario.",
    "Contigo aprendí que lo pequeño también es infinito: una mirada, un beso virtual(por ahora).",
    "Quiero ser tu complicidad para siempre (y tu proveedor de cafés).",
    "Me gustas hasta cuando no lo haces a propósito.",
    "Prometo cuidarte en los días de lluvia y celebrar contigo los días de sol.",
    "Eres mi lugar seguro y mi aventura preferida.",
    "Gracias por ser tan tú. Te admiro y te adoro.",
    "Aquí va la última notita: te amo, hoy y todos los días. ❤️",
]

# --- Domingo: mensajes especiales para un domingo amoroso ---
SUNDAY_NOTES = [
    "Feliz domingo, mi amor. Hoy preparo un brunch para tu sonrisa desde la distancia.",
    "Que este domingo te llegue suave como el café y cálido como mis abrazos.",
    "Hoy el plan es descansar, soñar y pensar en todos nuestros futuros domingos juntos.",
    "Que tu día sea tan dulce como tu risa y tan tranquilo como tu mirada.",
    "Domingo de calma: recuerda que soy tu equipo en todo, siempre contigo.",
    "Si pudiera, llenaría tu ventana de luz y tu taza de ternura. Buen domingo.",
    "Hoy prometo enviarte mil mensajitos cariñosos y una playlist para bailar en la cocina.",
    "Descansa mi vida, que hoy yo cuido tus pensamientos bonitos.",
    "Imagina que estoy ahí con un abrazo largo y una sonrisa cómplice. Feliz domingo.",
    "Domingo para agradecer: gracias por existir y por ser mi persona favorita.",
    "Hoy dejo una nota para ti: te mereces paz, amor y mil pequeños momentos felices.",
    "Cierra los ojos y escucha: te digo te amo hasta en el silencio. ❤️",
]


class UnlockRequest(BaseModel):
    date: str


def _parse_date(raw: str) -> date:
    try:
        return date.fromisoformat(raw)
    except ValueError as exc:
        raise HTTPException(status_code=400, detail="Fecha inválida. Usa AAAA-MM-DD.") from exc


PAGE_HTML = (
    """
<!doctype html>
<html lang="es">
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width,initial-scale=1" />
<title>Para {RECIPIENT_NAME} 🌹</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Great+Vibes&family=Inter:wght@300;400;600;700&display=swap" rel="stylesheet">
<style>
  :root{--bg:#fff6fb;--card:#ffffff;--accent:#d63384;--muted:#6b7280;--soft:#fdf2f8}
  *{box-sizing:border-box}
  body{font-family:Inter, Arial, sans-serif;background:linear-gradient(180deg,#fff6fb 0%, #fff 100%);margin:0;padding:32px;display:flex;justify-content:center;align-items:center;min-height:100vh}
  .wrap{width:960px;max-width:96%;background:var(--card);border-radius:20px;box-shadow:0 20px 60px rgba(30,20,30,0.08);padding:28px;display:grid;grid-template-columns:320px 1fr;gap:24px}
  .left{display:flex;flex-direction:column;gap:16px;align-items:center;padding:12px}
  .rose{font-size:56px;transform:translateY(-4px)}
  h1{font-family:'Great Vibes', cursive;font-size:48px;margin:6px 0;color:var(--accent)}
  .subtitle{color:var(--muted);font-size:14px;text-align:center}
  .recipient{background:linear-gradient(90deg, rgba(214,51,132,0.06), rgba(255,240,245,0.06));padding:10px 14px;border-radius:12px;border:1px solid rgba(214,51,132,0.06);font-weight:600;color:#4a2b3a}
  .note_card{background:linear-gradient(180deg,#fff 0%, #fff9fb 100%);border-radius:16px;padding:26px;box-shadow:0 10px 30px rgba(200,150,170,0.06);min-height:260px;display:flex;flex-direction:column;justify-content:center;align-items:center}
  .note_text{font-size:22px;line-height:1.4;color:#3b2a3a;font-family:'Great Vibes', cursive;text-align:center;padding:8px}
  .controls{display:flex;justify-content:space-between;align-items:center;margin-top:16px}
  .btn{padding:10px 14px;border-radius:12px;border:0;cursor:pointer;font-weight:600;transition:transform .12s ease, box-shadow .12s ease}
  .btn:active{transform:translateY(1px)}
  .btn.primary{background:var(--accent);color:#fff;box-shadow:0 8px 18px rgba(214,51,132,0.18)}
  .btn.secondary{background:transparent;border:1px solid #f6d7e6;color:var(--accent)}
  .progress{font-size:13px;color:var(--muted)}
  .footer{font-size:13px;color:var(--muted);text-align:center;margin-top:14px}
  .heart{color:#e11d6b;font-size:20px}
  .small-hint{font-size:12px;color:var(--muted);margin-top:6px}
  .spark{display:inline-block;transform-origin:center center}
  .lock-box{background:var(--soft);border:1px dashed rgba(214,51,132,0.35);padding:12px 14px;border-radius:14px;width:100%;display:flex;flex-direction:column;gap:10px;align-items:flex-start}
  .lock-title{font-weight:700;color:#4a2b3a;display:flex;gap:8px;align-items:center}
  .lock-input{padding:10px 12px;border-radius:10px;border:1px solid #f6d7e6;width:100%}
  .status{font-size:13px;color:var(--muted);min-height:18px}
  .status.ok{color:#0f9d58;font-weight:700}
  .status.error{color:#d63384;font-weight:700}
  @media (max-width:820px){.wrap{grid-template-columns:1fr;}.left{order:2}}
</style>
</head>
<body>
<div class="wrap">
  <div class="left">
    <div class="rose" id="lock_icon">🔒</div>
    <h1>Para {RECIPIENT_NAME}</h1>
    <div class="subtitle">Unas notitas hechas con todo mi cariño</div>

    <div style="margin-top:12px;display:flex;flex-direction:column;gap:10px;align-items:center;width:100%">
      <div class="recipient">{RECIPIENT_NICK}</div>
      <div class="lock-box">
        <div class="lock-title">🔑 Desbloquea con la fecha en que empezamos: </div>
        <input id="date_input" class="lock-input" type="date" />
        <button id="unlock_btn" class="btn secondary" style="width:100%">Abrir candadito</button>
        <div id="status" class="status">Formato: AAAA-MM-DD</div>
      </div>
      <div class="small-hint">Una vez desbloqueado, podrás leer nuestras 12 notitas.</div>
      <button id="start_btn" class="btn primary" style="margin-top:8px;width:160px" disabled>Ver notitas</button>
    </div>

    <div style="margin-top:18px;text-align:center">
      <div class="progress" id="progress_label">—</div>
    </div>
  </div>

  <div>
    <div class="note_card" id="note_card">
      <div class="note_text" id="note_text">Pulsa "Abrir candadito" con la fecha correcta para recibir una sorpresa 💌</div>
    </div>
    <div class="controls">
      <button class="btn secondary" id="prev_btn" disabled>&larr; Anterior</button>
      <div style="display:flex;gap:10px;align-items:center">
        <button class="btn primary" id="next_btn" disabled>Siguiente &rarr;</button>
        <button class="btn secondary" id="reset_btn">Reiniciar</button>
      </div>
    </div>
    <div class="footer">Con amor, tu persona favorita <span class="heart">❤</span></div>
  </div>
</div>

<script>
let notes = [];
let index = -1;
let unlocked = false;
const RECIPIENT = "{RECIPIENT_NAME}";

async function loadNotes(){
  const r = await fetch('/notes');
  if (!r.ok) return [];
  return await r.json();
}

function personalize(text){
  return text.replace(/\{\{name\}\}/g, RECIPIENT);
}

function render(){
  const textEl = document.getElementById('note_text');
  const progress = document.getElementById('progress_label');
  const prev = document.getElementById('prev_btn');
  const next = document.getElementById('next_btn');
  const start = document.getElementById('start_btn');

  start.disabled = !unlocked;
  next.disabled = !unlocked || index === notes.length - 1;
  prev.disabled = !unlocked || index <= 0;

  if (!unlocked){
    textEl.innerHTML = 'Pulsa "Abrir candadito" con la fecha correcta para recibir una sorpresa 💌';
    progress.textContent = `—`;
    return;
  }

  if (index < 0 || index >= notes.length){
    textEl.innerHTML = '{UNLOCK_NOTE}';
    progress.textContent = `—`;
    return;
  }

  const item = notes[index];
  const personalized = personalize(item.text);
  textEl.innerHTML = personalized;
  progress.textContent = `Nota ${index+1} de ${notes.length}`;
}

async function tryUnlock(){
  const input = document.getElementById('date_input');
  const status = document.getElementById('status');
  const icon = document.getElementById('lock_icon');
  if (!input.value){
    status.textContent = 'Escribe la fecha en formato AAAA-MM-DD';
    status.className = 'status error';
    return;
  }

  const resp = await fetch('/unlock', {
    method: 'POST',
    headers: {'Content-Type': 'application/json'},
    body: JSON.stringify({date: input.value})
  });

  const data = await resp.json();
  if (resp.ok && data.unlocked){
    unlocked = true;
    status.textContent = data.message;
    status.className = 'status ok';
    icon.textContent = '🔓';
    index = -1;
    render();
  } else {
    unlocked = false;
    status.textContent = data.detail || 'Fecha incorrecta, intenta otra vez ❤';
    status.className = 'status error';
    icon.textContent = '🔒';
    render();
  }
}

window.addEventListener('load', async ()=>{
  notes = await loadNotes();
  render();
});

document.getElementById('unlock_btn').onclick = ()=>{ tryUnlock(); };
document.getElementById('start_btn').onclick = ()=>{ if (!unlocked) return; index = 0; render(); };
document.getElementById('next_btn').onclick = ()=>{ if (unlocked && index < notes.length - 1) index++; render(); };
document.getElementById('prev_btn').onclick = ()=>{ if (unlocked && index > 0) index--; render(); };
document.getElementById('reset_btn').onclick = ()=>{ index = -1; render(); };
</script>
</body>
</html>
"""
).replace("{RECIPIENT_NAME}", RECIPIENT_NAME).replace("{RECIPIENT_NICK}", RECIPIENT_NICK)


@app.get('/notes')
async def get_notes():
    items = []
    for i, text in enumerate(NOTES, start=1):
        items.append({"id": i, "text": text})
    return JSONResponse(items)


@app.get('/notes/domingo')
async def get_sunday_notes():
    items = []
    for i, text in enumerate(SUNDAY_NOTES, start=1):
        items.append({"id": i, "text": text})
    return JSONResponse(items)


@app.post('/unlock')
async def unlock(payload: UnlockRequest):
    chosen_date = _parse_date(payload.date)
    if chosen_date != ANNIVERSARY_DATE:
        raise HTTPException(status_code=401, detail="Esa no es la fecha, intenta de nuevo.")
    return {"unlocked": True, "message": UNLOCK_NOTE}


@app.get('/love', response_class=HTMLResponse)
async def love_page():
    return HTMLResponse(PAGE_HTML)


@app.get('/')
async def root():
    return RedirectResponse(url='/love')


if __name__ == '__main__':
    import uvicorn

    uvicorn.run(app, host='0.0.0.0', port=8080)
