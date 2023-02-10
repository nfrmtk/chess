# Start via `make test-debug` or `make test-release`
async def test_basic(service_client):
    response = await service_client.post('/hello',
                                         params={'name': 'Tester'})
    assert response.status == 200
    assert response.text == 'Hello, Tester!\n'


async def test_create_room(service_client):
    response = await service_client.post('/v1/devroom/make',
                                         params={'name': 'TesterWhites'})
    assert response.status == 200

    response = await service_client.post('/v1/devroom/make',
                                         params={'name': 'TesterBlacks'})
    assert response.status == 403

    response = await service_client.post('/v1/devroom/join',
                                         params={'name': 'TesterBlacks'})
    assert response.status == 200


async def test_make_move(service_client):
    await service_client.post('/v1/devroom/make',
                              params={'name': 'TesterWhites'})
    await service_client.post('/v1/devroom/join',
                              params={'name': 'TesterBlacks'})
    response = await service_client.post('/v1/devroom/move', json={
        'previous': 'E2',
        'next': 'E4'
    }, params={'name': 'TesterBlacks'})
    assert response.status == 400
    response = await service_client.post('/v1/devroom/move', json={
        'previous': 'E2',
        'next': 'E4'
    }, params={'name': 'TesterWhites'})
    assert response.status == 200

    response = await service_client.get('/v1/devroom/move',
                                        params={'name': 'TesterWhites'})
    assert response.json()['last-move'] == '!#'
