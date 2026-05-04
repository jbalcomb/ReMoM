let stdin = "";
process.stdin.on("data", chunk => stdin += chunk);
process.stdin.on("end", () => {
  let filePath = "a file";
  try { filePath = JSON.parse(stdin).tool_input?.file_path || filePath; } catch (e) { /* still emit reminder on malformed input */ }
  process.stderr.write(`You just edited ${filePath}. Before declaring this task complete, verify the change works — run/build/test as appropriate for this file type. If verification doesn't apply (docs, generated artifact, etc.), state that explicitly in your next message. Do not say "Done.", "It works.", or "Created." for this edit without evidence from a tool call.\n`);
  process.exit(2);
});
