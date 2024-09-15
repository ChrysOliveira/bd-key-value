function ValidaCpf(cpf)
    local numeros = {}

    -- Percorre a string cpf e converte cada caractere em número
    for i = 1, #cpf do
        numeros[i] = tonumber(cpf:sub(i, i)) -- Usando string.sub para acessar cada caractere
    end

    -- Verifica se o array numeros tem exatamente 11 dígitos
    if #numeros ~= 11 then
        error("O tamanho do CPF informado é diferente de 11: " .. ArrayToString(numeros))
    end

    local validacao_primeiro_digito = VerificaDigito(numeros, numeros[10], true)
    local validacao_segundo_digito = VerificaDigito(numeros, numeros[11], false)

    if validacao_primeiro_digito and validacao_segundo_digito then
        return true
    else
        error("O CPF" .. cpf .. " nao e valido")
    end
end

function ValidaData(data)
    if string.len(data) ~= 10 then
        error("A data informada ou esta incompleta ou possui valores a mais. Favor informar a data no padrao YYYY-MM-DD")
    end

    if data:sub(5, 5) ~= "-" or data:sub(8, 8) ~= "-" then
        error("A data informada nao esta no padrao ISO8601. Favor informar a data no padrao YYYY-MM-DD")
    end

    if
        not data:match("^%d%d%d%d%-%d%d%-%d%d$")
        or tonumber(data:sub(6, 7)) > 12
        or tonumber(data:sub(6, 7)) < 1
        or tonumber(data:sub(9, 10)) > 31
        or tonumber(data:sub(9, 10)) < 1
    then
        error("A data informada possui valores invalidos. Favor informar a data no padrao YYYY-MM-DD")
    end

    return true
end

function VerificaDigito(array_cpf, digito, primeiro)
    local acumulado = 0

    local tamanho_validacao = 0
    local primeiro_valor_multiplicacao = 0

    if primeiro then
        tamanho_validacao = 9
        primeiro_valor_multiplicacao = 10
    else
        tamanho_validacao = 10
        primeiro_valor_multiplicacao = 11
    end
    for i = 1, tamanho_validacao do
        acumulado = acumulado + (array_cpf[i] * (primeiro_valor_multiplicacao))
        primeiro_valor_multiplicacao = primeiro_valor_multiplicacao - 1
    end

    local divisao = acumulado % 11

    local subtracao = 11 - divisao

    if subtracao >= 10 then
        return 0 == digito
    else
        return subtracao == digito
    end
end

function MascaraCPF(cpf)
    return cpf:sub(1, 3) .. "." .. cpf:sub(4, 6) .. "." .. cpf:sub(7, 9) .. "-" .. cpf:sub(10, 11)
end

function MascaraData(data)
    return data:sub(9,10) .. "/" .. data:sub(6,7) .. "/" .. data:sub(1,4)
end

function ArrayToString(array)
    local result = "{"
    for i = 1, #array do
        result = result .. array[i]
        if i < #array then
            result = result .. ", "
        end
    end
    result = result .. "}"
    return result
end
