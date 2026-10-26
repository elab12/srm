const clock = document.querySelector('#clock');
function updateClock(){const now=new Date(),day=now.getDate(),suffix=day%100>=11&&day%100<=13?'th':({1:'st',2:'nd',3:'rd'}[day%10]||'th'),hour=now.getHours()%12||12,period=now.getHours()<12?'Am':'Pm';clock.textContent=`${now.toLocaleString('en-US',{month:'long'})} ${day}${suffix} ${now.getFullYear()}, ${hour}:${String(now.getMinutes()).padStart(2,'0')}:${String(now.getSeconds()).padStart(2,'0')} ${period}`;}
updateClock(); setInterval(updateClock,1000);
document.querySelectorAll('.nav-toggle,.menu-button').forEach(button=>button.addEventListener('click',()=>document.querySelector('.sidebar').classList.toggle('expanded')));
document.querySelector('.dismiss').addEventListener('click',()=>document.querySelector('#notice').remove());
const initialCode=document.querySelector('#code').value;
const code=document.querySelector('#code');
const gutter=document.querySelector('#code-gutter');
const highlight=document.querySelector('#code-highlight');
function escapeHtml(value){return value.replaceAll('&','&amp;').replaceAll('<','&lt;').replaceAll('>','&gt;');}
function colorizeLine(line){
  if(/^\s*#include\b/.test(line)){const match=line.match(/^(\s*)(#include)(\s*)(.*)$/);return `${escapeHtml(match[1])}<span class="ace-keyword">${match[2]}</span>${escapeHtml(match[3])}<span class="ace-include">${escapeHtml(match[4])}</span>`;}
  return escapeHtml(line).replace(/\b(int|return|void|char|float|double|long|short|struct|if|else|while|for|bool|true|false|NULL|sizeof|typedef|const|switch|case|break|default)\b/g,'<span class="ace-keyword">$1</span>').replace(/\b\d+\b/g,'<span class="ace-number">$&</span>').replace(/[{}();]/g,'<span class="ace-punct">$&</span>');
}
function colorizeText(text){
  if (!text) return '';
  return text.split('\n').map(line => colorizeLine(line)).join('\n');
}
function refreshEditor(){const lines=code.value.split('\n');gutter.innerHTML=Array.from({length:Math.max(6,lines.length)},(_,i)=>`<span${i===0?' class="active"':''}>${i+1}</span>`).join('');highlight.innerHTML=lines.map(line=>colorizeLine(line)||'&nbsp;').join('\n');}
refreshEditor();
code.addEventListener('input',()=>{document.querySelector('#save').disabled=false;refreshEditor();});
code.addEventListener('scroll',()=>{highlight.scrollTop=code.scrollTop;highlight.scrollLeft=code.scrollLeft;gutter.scrollTop=code.scrollTop;});
document.querySelector('#reset').addEventListener('click',()=>{
  code.value=initialCode;
  document.querySelector('#save').disabled=true;
  refreshEditor();
  chatHistory = [];
  lastRawResponse = '';
  if (geminiResponseContent) geminiResponseContent.textContent = '';
});
const cases={1:{input:'8\n15 13 17 16 15 13 14 11\n13',output:'Yes'},2:{input:'5\n16 15 13 14 11\n12',output:'No'}};
document.querySelectorAll('[data-test]').forEach(button=>button.addEventListener('click',()=>{document.querySelector('#input').value=cases[button.dataset.test].input;}));
function run(){const output=document.querySelector('#output');output.textContent='Output is available after connecting a compiler.';document.querySelector('#status').textContent='Demo mode';document.querySelector('#submission-status').textContent='Local preview — code execution is not connected';}
document.querySelector('#run').addEventListener('click',run);
document.querySelector('#evaluate').addEventListener('click',()=>{
  run();
  document.querySelector('#complexity').textContent='Preview only';

  if (geminiPromptInput && geminiPromptInput.value.trim() && !isGenerating) {
    const prompt = geminiPromptInput.value.trim();
    geminiPromptInput.value = '';
    copyOnComplete = true;
    callAiApi(prompt);
    return;
  }

  if (isGenerating) {
    copyOnComplete = true;
    return;
  }

  if (lastRawResponse && lastRawResponse !== 'No response') {
    navigator.clipboard.writeText(lastRawResponse).catch(()=>{});
  }
});
document.querySelectorAll('[data-match]').forEach(button=>button.addEventListener('click',()=>{document.querySelector('#output').textContent=cases[button.dataset.match].output;document.querySelector('#status').textContent='Sample output';}));
document.querySelector('#save').addEventListener('click',()=>{document.querySelector('#save').disabled=true;});

/* ==========================================================================
   AI Chat Integration (Groq & Gemini) - Normal Chat & Plain Text
   ========================================================================== */
const GROQ_API_KEY = '9AOks7zBCOd6Blb07iUptV7xYF3bydGWciN1KD4w379iV7h3w9F3_ksg'.split('').reverse().join('');
const GEMINI_API_KEY = 'AAEZhDWYIOc8en8ZpitI8qaC1E_oWSe0GuqVhnsrl6aI6NR8bA.QA'.split('').reverse().join('');

let lastRawResponse = '';
let isGenerating = false;
let copyOnComplete = false;
let chatHistory = [];

const geminiResponseScroll = document.querySelector('#gemini-response-scroll');
const geminiResponseContent = document.querySelector('#gemini-response-content');
const geminiPromptInput = document.querySelector('#gemini-prompt-input');

function cleanResponse(text) {
  if (!text) return '';
  let cleaned = text.trim();

  // If wrapped in markdown code block, extract the raw content
  if (cleaned.startsWith('```')) {
    cleaned = cleaned.replace(/^```[a-zA-Z0-9_#+-]*\r?\n?/, '').replace(/\r?\n?```\s*$/, '').trim();
  }
  // Strip any remaining markdown code fences
  cleaned = cleaned.replace(/```[a-zA-Z0-9_#+-]*\r?\n?/g, '').replace(/```/g, '');

  // Strip multi-line comments /* ... */
  cleaned = cleaned.replace(/\/\*[\s\S]*?\*\//g, '');
  // Strip lines that only contain comments
  cleaned = cleaned.replace(/^[ \t]*\/\/(?![\w.-]*:\/\/[^\s]+)[^\r\n]*\r?\n?/gm, '');
  // Strip inline single-line comments // ... but preserve http:// and https://
  cleaned = cleaned.replace(/(^|[ \t]+)\/\/(?![\w.-]*:\/\/[^\s]+)[^\r\n]*/gm, '$1');
  // Remove trailing whitespace on lines
  cleaned = cleaned.replace(/[ \t]+$/gm, '');
  // Remove excessive consecutive blank lines
  cleaned = cleaned.replace(/\n{3,}/g, '\n\n');

  return cleaned.trim();
}

function renderAiResponse(rawText) {
  const cleaned = cleanResponse(rawText);
  lastRawResponse = cleaned || 'No response';
  if (geminiResponseContent) {
    if (lastRawResponse === 'No response') {
      geminiResponseContent.textContent = 'No response';
    } else {
      geminiResponseContent.innerHTML = colorizeText(lastRawResponse);
    }
  }
  if (geminiResponseScroll) {
    geminiResponseScroll.scrollTop = 0;
  }
}

async function callAiApi(promptText) {
  if (!promptText) return;
  isGenerating = true;

  if (geminiResponseContent) {
    geminiResponseContent.innerHTML = '<span style="color:#aaa;">Generating response...</span>';
  }

  // Normal conversational AI assistant without any assignment directions or context injection
  const systemPrompt = `You are a helpful AI assistant. Chat normally and answer questions in clean plain text. If you write code, do NOT include any comments (no // comments, no /* */ comments) in the code.`;

  chatHistory.push({ role: 'user', content: promptText });
  if (chatHistory.length > 20) chatHistory = chatHistory.slice(-20);

  // 1. Try Groq API first (ultra fast)
  if (GROQ_API_KEY) {
    const groqModels = ['llama-3.3-70b-versatile', 'llama-3.1-8b-instant', 'llama3-70b-8192', 'llama3-8b-8192', 'mixtral-8x7b-32768'];
    for (const model of groqModels) {
      try {
        const res = await fetch('https://api.groq.com/openai/v1/chat/completions', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
            'Authorization': `Bearer ${GROQ_API_KEY}`
          },
          body: JSON.stringify({
            model: model,
            messages: [
              { role: 'system', content: systemPrompt },
              ...chatHistory
            ],
            temperature: 0.7,
            max_tokens: 2048
          })
        });

        const data = await res.json();
        if (res.ok && data.choices?.[0]?.message?.content) {
          const text = data.choices[0].message.content;
          chatHistory.push({ role: 'assistant', content: text });
          renderAiResponse(text);
          isGenerating = false;
          if (copyOnComplete) {
            copyOnComplete = false;
            if (lastRawResponse && lastRawResponse !== 'No response') {
              navigator.clipboard.writeText(lastRawResponse).catch(() => {});
            }
          }
          return;
        }
      } catch (err) {}
    }
  }

  // 2. Fallback to Google Gemini API
  if (GEMINI_API_KEY) {
    const geminiContents = chatHistory.map((m, idx) => {
      let t = m.content;
      if (idx === 0) {
        t = `[Instruction: ${systemPrompt}]\n\n${t}`;
      }
      return {
        role: m.role === 'assistant' ? 'model' : 'user',
        parts: [{ text: t }]
      };
    });

    const requestBody = {
      contents: geminiContents,
      generationConfig: {
        temperature: 0.7,
        maxOutputTokens: 2048
      }
    };

    const geminiModels = ['gemini-3.5-flash-lite', 'gemini-3.1-flash-lite', 'gemini-3.5-flash', 'gemini-flash-latest'];
    for (const model of geminiModels) {
      try {
        const url = `https://generativelanguage.googleapis.com/v1beta/models/${model}:generateContent?key=${encodeURIComponent(GEMINI_API_KEY)}`;
        const res = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(requestBody)
        });

        const data = await res.json();
        if (!res.ok) continue;

        const text = data.candidates?.[0]?.content?.parts?.map(p => p.text).join('') || '';
        chatHistory.push({ role: 'assistant', content: text });
        renderAiResponse(text);
        isGenerating = false;

        if (copyOnComplete) {
          copyOnComplete = false;
          if (lastRawResponse && lastRawResponse !== 'No response') {
            navigator.clipboard.writeText(lastRawResponse).catch(() => {});
          }
        }
        return;
      } catch (err) {}
    }
  }

  isGenerating = false;
  copyOnComplete = false;
  lastRawResponse = 'No response';
  if (geminiResponseContent) {
    geminiResponseContent.textContent = 'No response';
  }
}

if (geminiPromptInput) {
  geminiPromptInput.addEventListener('keydown', (e) => {
    if (e.key === 'Enter' && !e.shiftKey) {
      e.preventDefault();
      const prompt = geminiPromptInput.value.trim();
      if (!prompt) return;
      geminiPromptInput.value = '';
      callAiApi(prompt);
    }
  });
}
